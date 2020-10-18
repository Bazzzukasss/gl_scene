#include "gl_scene_camera.h"
#include "gl_scene_utility.h"
#include "gl_scene_defaults.h"
#include <QtMath>

using namespace gl_scene;
using namespace gl_scene::defaults;

#define CHECK_RANGE_LIMIT(value, range) \
    if (mRangeLimits.range.isEnabled) \
    { \
        if (value > mRangeLimits.range.max) \
        { \
            value = mRangeLimits.range.max; \
        } \
        if (value < mRangeLimits.range.min) \
        { \
            value = mRangeLimits.range.min; \
        } \
    }

Camera::Camera(const Camera& camera) :
    Camera(camera.mPosition, camera.mWorldUp, camera.mYaw, camera.mPitch, camera.mSpeed, camera.mSensitivity,
           camera.mZoom, camera.mScale, camera.mProjectionType, camera.mProjectionPerspective, camera.mProjectionOrtho,
           camera.mRangeLimits)
{}

Camera::Camera(const Vec3& position, const Vec3& up, float yaw, float pitch, float speed, float sensitivity, float zoom,
               const Vec3& scale, Projection::Type proj_type, const ProjectionPerspective& proj_perspective,
               const ProjectionOrtho& proj_ortho, const RangeLimits& range_limits) :
    mYaw(yaw),
    mPitch(pitch),
    mSpeed(speed),
    mSensitivity(sensitivity),
    mZoom(zoom),
    mScale(scale),
    mPosition(position),
    mWorldUp(up),
    mRangeLimits(range_limits),
    mProjectionPerspective(proj_perspective),
    mProjectionOrtho(proj_ortho)
{
    setProjectionType(proj_type);
    update();
}

Camera::Camera(const Vec3& position, float yaw, float pitch, float zoom, const Vec3& scale,
               Projection::Type proj_type) :
    Camera(position, cameras::kDefault.mUp, yaw, pitch, cameras::kDefault.mSpeed, cameras::kDefault.mSensitivity, zoom,
           scale, proj_type, cameras::kDefault.mProjectionPerspective, cameras::kDefault.mProjectionOrtho,
           cameras::kDefault.mRangeLimits)
{}

void Camera::move(MovementDirection direction, float delta)
{
    float k = mCurrentProjection->getProjectionKoef(mPosition.z(), mViewPortSize.first);
    switch (direction)
    {
        case MovementDirection::kXYForward:
            moveXYFront(delta * k);
            break;
        case MovementDirection::kForward:
            moveFront(mSpeed * delta);
            break;
        case MovementDirection::kRight:
            moveRight(delta * k);
            break;
        case MovementDirection::kUp:
            moveUp(delta * k);
            break;
    }
}

void Camera::moveFront(float delta)
{
    move(mFront * delta);
}

void Camera::moveXYFront(float delta)
{
    move(mXYFront * delta);
}

void Camera::moveUp(float delta)
{
    move(mUp * delta);
}

void Camera::moveRight(float delta)
{
    move(mRight * delta);
}

void Camera::move(const Vec3& delta)
{
    mPosition += delta;
    update(false, true);
}

void Camera::rotateAround(int delta_x, int delta_y)
{
    rotateAround(static_cast<float>(delta_x) * mSensitivity, static_cast<float>(delta_y) * mSensitivity);
}

void Camera::rotateAround(float delta_yaw, float delta_pitch)
{
    auto oldFront = mFront;
    rotate(delta_yaw, delta_pitch);
    mPosition -= (mFront - oldFront) * mLook.length();

    update(false);
}

void Camera::rotate(int delta_x, int delta_y)
{
    rotate(static_cast<float>(delta_x) * mSensitivity, static_cast<float>(delta_y) * mSensitivity);
}

void Camera::rotate(float delta_yaw, float delta_pitch)
{
    mYaw += delta_yaw;
    mPitch += delta_pitch;

    update();
}

void Camera::zoom(float delta)
{
    mZoom -= delta * mSensitivity;

    update(false);
}

void Camera::scale(const Vec3& delta_scale)
{
    mScale += delta_scale * mSensitivity;

    update(false, false);
}

void Camera::focus(const Vec3& point)
{
    auto delta = point - mLookPoint;
    delta.setZ(0.0f);
    mPosition += delta;

    update(false, true);
}

void Camera::switchProjection()
{
    if (mProjectionType == Projection::Type::kPerspective)
    {
        setProjectionType(Projection::Type::kOrtho);
    }
    else
    {
        setProjectionType(Projection::Type::kPerspective);
    }
}

void Camera::setProjectionType(Projection::Type proj_type)
{
    mProjectionType    = proj_type;
    mCurrentProjection = proj_type == Projection::Type::kPerspective
        ? static_cast<gl_scene::Projection*>(&mProjectionPerspective)
        : static_cast<gl_scene::Projection*>(&mProjectionOrtho);

    update(false, false);
}

void Camera::setViewPort(int veiw_width, int view_height)
{
    float ratio = static_cast<float>(veiw_width) / static_cast<float>(view_height);
    mProjectionPerspective.setRatio(ratio);
    mProjectionOrtho.setRatio(ratio);
    mViewPortSize = {veiw_width, view_height};
    update(false, true);
}

void Camera::setPerspective(const ProjectionPerspective& perspective)
{
    mProjectionPerspective = perspective;
    calculateViewMatrix();
}

void Camera::setOrtho(const ProjectionOrtho& ortho)
{
    mProjectionOrtho = ortho;
    calculateViewMatrix();
}

void Camera::update(bool use_angles, bool update_look)
{
    checkRangeLimits();

    if (use_angles)
    {
        float yaw   = qDegreesToRadians(mYaw);
        float pitch = qDegreesToRadians(mPitch);

        mFront.setX(cosf(yaw) * cosf(pitch));
        mFront.setY(sinf(yaw) * cosf(pitch));
        mFront.setZ(sinf(pitch));
        mFront.normalize();

        mRight = QVector3D::crossProduct(mFront, mWorldUp);
        mRight.normalize();

        mUp = QVector3D::crossProduct(mRight, mFront);
        mUp.normalize();

        auto xyMove = mFront + mUp;
        mXYFront    = {xyMove.x(), xyMove.y(), 0.0f};
    }

    calculateViewMatrix();

    if (update_look)
    {
        mLookPoint = toWorldXYCoordinates(mViewPortSize.first / 2, mViewPortSize.second / 2);
        mLook      = getPosition() - mLookPoint;
    }

    mProjectionOrtho.setScale(mScale);

    emit signalChanged(getTransformation());
}

void Camera::calculateViewMatrix()
{
    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(mPosition, mPosition + mFront, mUp);
    mViewMatrix.scale(mZoom);
}

void Camera::checkRangeLimits()
{
    float positionX{mPosition.x()};
    float positionY{mPosition.y()};
    float positionZ{mPosition.z()};
    float scaleX{mScale.x()};
    float scaleY{mScale.y()};
    float scaleZ{mScale.z()};

    CHECK_RANGE_LIMIT(mPitch, rangePitch)
    CHECK_RANGE_LIMIT(mYaw, rangeYaw)
    CHECK_RANGE_LIMIT(mZoom, rangeZoom)
    CHECK_RANGE_LIMIT(positionX, rangeX)
    CHECK_RANGE_LIMIT(positionY, rangeY)
    CHECK_RANGE_LIMIT(positionZ, rangeZ)
    CHECK_RANGE_LIMIT(scaleX, rangeScale)
    CHECK_RANGE_LIMIT(scaleY, rangeScale)
    CHECK_RANGE_LIMIT(scaleZ, rangeScale)

    mPosition.setX(positionX);
    mPosition.setY(positionY);
    mPosition.setZ(positionZ);
    mScale.setX(scaleX);
    mScale.setY(scaleY);
    mScale.setZ(scaleZ);
}

void Camera::setPitch(float pitch)
{
    mPitch = pitch;
    update();
}

void Camera::setYaw(float yaw)
{
    mYaw = yaw;
    update();
}

void Camera::setZoom(float zoom)
{
    mZoom = zoom;
    update();
}

void Camera::setScale(const Vec3& scale)
{
    mScale = scale;
    update(false, false);
}

void Camera::setPosition(const Vec3& position)
{
    mPosition = position;
    update(false, true);
}

Vec3 Camera::toWorldXYCoordinates(int screen_x, int screen_y, float world_z) const
{
    Vec3 worldNear = toWorldCoordinates(screen_x, screen_y, 0.0f);
    Vec3 worldFar  = toWorldCoordinates(screen_x, screen_y, 1.0f);

    if (worldFar.z() > worldNear.z() - 0.001f)
    {
        worldFar.setZ(worldNear.z() - 0.001f);
    }

    auto worldDir = worldFar - worldNear;
    float res     = !qFuzzyIsNull(worldDir.z()) ? (world_z - worldNear.z()) / worldDir.z() : world_z - worldNear.z();

    auto worldX = !qFuzzyIsNull(worldDir.x()) ? res * worldDir.x() + worldNear.x() : res + worldNear.x();
    auto worldY = !qFuzzyIsNull(worldDir.y()) ? res * worldDir.y() + worldNear.y() : res + worldNear.y();
    auto worldZ = world_z;

    Vec3 pos = {worldX, worldY, worldZ};

    return pos;
}

Point3Pack Camera::toWorldXYCoordinates(const Point2Pack& screen_points, float world_z) const
{
    Point3Pack points;
    for (const auto& point : screen_points)
    {
        points.push_back(gl_scene::toPoint3(toWorldXYCoordinates(point.first, point.second, world_z)));
    }
    return points;
}

Vec3 Camera::toWorldCoordinates(int screen_x, int screen_y, float distance) const
{
    int screenW = mViewPortSize.first;
    int screenH = mViewPortSize.second;
    Vec3 screenNear(screen_x, screenH - screen_y, distance);

    return screenNear.unproject(getView(), getProjection(), QRect(0, 0, screenW, screenH));
}

Point3Pack Camera::toWorldCoordinates(const Point2Pack& screen_points, float distance) const
{
    Point3Pack points;
    for (const auto& point : screen_points)
    {
        points.push_back(gl_scene::toPoint3(toWorldCoordinates(point.first, point.second, distance)));
    }

    return points;
}

Vec2 Camera::toScreenCoordinates(const Vec3& world_point) const
{
    const auto& position = getTransformation() * QVector4D(world_point, 1.0);

    return {position.x(), position.y()};
}

void Camera::resetTo(const Camera& camera)
{
    float orthoRatio       = this->mProjectionOrtho.getRatio();
    float perspectiveRatio = this->mProjectionPerspective.getRatio();

    this->mPosition              = camera.mPosition;
    this->mLookPoint             = camera.mLookPoint;
    this->mYaw                   = camera.mYaw;
    this->mPitch                 = camera.mPitch;
    this->mZoom                  = camera.mZoom;
    this->mSpeed                 = camera.mSpeed;
    this->mSensitivity           = camera.mSensitivity;
    this->mProjectionOrtho       = camera.mProjectionOrtho;
    this->mProjectionPerspective = camera.mProjectionPerspective;
    this->mRangeLimits           = camera.mRangeLimits;
    this->mScale                 = camera.mScale;
    this->setProjectionType(camera.mProjectionType);

    this->mProjectionOrtho.setRatio(orthoRatio);
    this->mProjectionPerspective.setRatio(perspectiveRatio);

    update(true, true);
}
