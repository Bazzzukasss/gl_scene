#include "gl_scene_projection.h"
#include <QtMath>

using namespace gl_scene;

Projection::Projection(Projection::Type type, float ratio) : mType(type), mRatio(ratio)
{}

ProjectionOrtho::ProjectionOrtho(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z,
                                 float ratio, const Vec3& scale) :
    Projection(Projection::Type::kOrtho, ratio),
    mMinX(min_x),
    mMaxX(max_x),
    mMinY(min_y),
    mMaxY(max_y),
    mMinZ(min_z),
    mMaxZ(max_z),
    mScale(scale)
{
    calculate();
}

void ProjectionOrtho::setRatio(float ratio)
{
    mRatio = ratio;
    calculate();
}

void ProjectionOrtho::setXRange(float min, float max)
{
    mMinX = min;
    mMaxX = max;
    calculate();
}

void ProjectionOrtho::setYRange(float min, float max)
{
    mMinY = min;
    mMaxY = max;
    calculate();
}

void ProjectionOrtho::setZRange(float min, float max)
{
    mMinZ = min;
    mMaxZ = max;
    calculate();
}

void ProjectionOrtho::setScale(const Vec3& scale)
{
    mScale = scale;
    calculate();
}

float ProjectionOrtho::getProjectionKoef(float, int view_width) const
{
    return (mMaxX - mMinX) * mRatio * mScale.x() / static_cast<float>(view_width);
}

float ProjectionOrtho::getProjectedX(float x, int view_width) const
{
    return (mMinX + (mMaxX - mMinX) * x / view_width) * mScale.x() * mRatio;
}

void ProjectionOrtho::calculate()
{
    mProjection.setToIdentity();
    mProjection.ortho(mMinX * mRatio * mScale.x(), mMaxX * mRatio * mScale.x(), mMinY * mScale.y(), mMaxY * mScale.y(),
                      mMinZ * mScale.z(), mMaxZ * mScale.z());
}

ProjectionPerspective::ProjectionPerspective(float alfa, float near_distance, float far_distance, float ratio) :
    Projection(Projection::Type::kPerspective, ratio),
    mAlfa(alfa),
    mNear(near_distance),
    mFar(far_distance)
{
    calculate();
}

float ProjectionPerspective::getProjectionKoef(float camera_position, int view_width) const
{
    return camera_position * tanf(qDegreesToRadians(mAlfa)) * mRatio / static_cast<float>(view_width);
}

void ProjectionPerspective::setRatio(float ratio)
{
    mRatio = ratio;
    calculate();
}

void ProjectionPerspective::setAlfa(float alfa)
{
    mAlfa = alfa;
    calculate();
}

void ProjectionPerspective::setNear(float min_d)
{
    mNear = min_d;
    calculate();
}

void ProjectionPerspective::setFar(float max_d)
{
    mFar = max_d;
    calculate();
}

void ProjectionPerspective::calculate()
{
    mProjection.setToIdentity();
    mProjection.perspective(mAlfa, mRatio, mNear, mFar);
}
