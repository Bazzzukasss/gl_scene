#pragma once

#include "gl_scene_types.h"
#include "gl_scene_projection.h"

namespace gl_scene
{

/**
 * The Camera Class
 * @brief The class provides functionality to manipule the view port and the lookpoint
 */
class Camera : public QObject
{
    Q_OBJECT

 public:
    enum class MovementDirection
    {
        kForward,
        kRight,
        kUp,
        kXYForward
    };

    struct Range
    {
        bool isEnabled;
        float min;
        float max;
    };

    struct RangeLimits
    {
        Range rangeX;
        Range rangeY;
        Range rangeZ;
        Range rangeZoom;
        Range rangeYaw;
        Range rangePitch;
        Range rangeScale;
    };

    /**
     * @brief Copy constructor
     * @param camera - the instance of the camera to copy
     */
    Camera(const Camera& camera);

    /**
     * @brief Constructor for Camera
     * @param position - the initial camera's position
     * @param up - the vector to the world's up
     * @param yaw - the rotation angle which is relative to the camera's vertical axis
     * @param pitch - the rotation angle which is relative to the camera's lateral axis
     * @param speed - the coefficient of the camera's movement
     * @param sensitivity - the coefficient of the camera's rotation
     * @param zoom - the value of the camera's zoom
     * @param proj_type - the type of the current projection
     * @param proj_perspective - the data structure for perspective projection
     * @param proj_ortho - the data structure for orthogonal projection
     */
    Camera(const Vec3& position, const Vec3& up, float yaw, float pitch, float speed, float sensitivity, float zoom,
           const Vec3& scale, Projection::Type proj_type, const ProjectionPerspective& proj_perspective,
           const ProjectionOrtho& proj_ortho, const RangeLimits& range_limits);

    /**
     * @brief Constructor for Camera
     * @param position - the initial camera's position
     * @param yaw - the rotation angle which is relative to the camera's vertical axis
     * @param pitch - the rotation angle which is relative to the camera's lateral axis
     * @param zoom - the value of the camera's zoom
     * @param proj_type - the type of the current projection
     */
    Camera(const Vec3& position, float yaw, float pitch, float zoom, const Vec3& scale, Projection::Type proj_type);

    /**
     * @brief Moves the camera according to the direction by the delta value
     * @param direction - the movement direction vector
     * @param delta - the movement value in screen coordinates
     */
    void move(MovementDirection direction, float delta = 1.0f);

    /**
     * @brief Moves the camera by the delta value
     * @param delta - the movement vector
     */
    void move(const Vec3& delta);

    /**
     * @brief Moves the camera by the delta value in front direction
     * @param delta - the movement value
     */
    void moveFront(float delta);

    /**
     * @brief Moves the camera by the delta value in front direction on XY plane
     * @param delta - the movement value
     */
    void moveXYFront(float delta);

    /**
     * @brief Moves the camera by the delta value in up direction
     * @param delta - the movement value
     */
    void moveUp(float delta);

    /**
     * @brief Moves the camera by the delta value in right direction
     * @param delta - the movement value
     */
    void moveRight(float delta);

    /**
     * @brief Rotate the camera around its mounting point
     * @param delta_x - the mouse cursor x coordinate delta
     * @param delta_y - the mouse cursor y coordinate delta
     * @todo possibly, either find better solution or remove it alltogether
     */
    void rotate(int delta_x, int delta_y);

    /**
     * @brief Rotate the camera around its mounting point
     * @param delta_yaw - yaw angle delta
     * @param delta_pitch - pitch angle delta
     * @todo possibly, either find better solution or remove it alltogether
     */
    void rotate(float delta_yaw, float delta_pitch);

    /**
     * @brief Rotate the camera around z axis
     * @param delta_x - the mouse cursor x coordinate delta
     * @param delta_y - the mouse cursor y coordinate delta
     * @todo possibly, either find better solution or remove it alltogether
     */
    void rotateAround(int delta_x, int delta_y);

    /**
     * @brief Rotate the camera around z axis
     * @param delta_yaw - yaw angle delta
     * @param delta_pitch - pitch angle delta
     * @todo possibly, either find better solution or remove it alltogether
     */
    void rotateAround(float delta_yaw, float delta_pitch);

    /**
     * @brief Zoom the camera's view
     * @param delta - the zoom delta value
     */
    void zoom(float delta);

    /**
     * @brief Scale the camera's orthographic projection
     * @param delta_scale - the delta scale by x, y and z axis delta value
     */
    void scale(const Vec3& delta_scale);

    /**
     * @brief Focus the camera's view
     * @param point - the point camra should be focused on
     */
    void focus(const gl_scene::Vec3& point);

    /**
     * @brief Switch the camera bitween orthogonal and perspective projection
     */
    void switchProjection();

    /**
     * @brief Projects the screen's point to the OXY plane
     * @param screen_x - the screen's point x coordinate
     * @param screen_y - the screen's point y coordinate
     * @param world_z - the z coordinate of the OXY plane
     * @return Point in world coordinates
     */
    gl_scene::Vec3 toWorldXYCoordinates(int screen_x, int screen_y, float world_z = 0.0f) const;

    /**
     * @brief Projects the screen's points to the OXY plane
     * @param screen_points - the container with screen's points
     * @param world_z - the z coordinate of the OXY plane
     * @return Point's in  world coordinates
     */
    gl_scene::Point3Pack toWorldXYCoordinates(const gl_scene::Point2Pack& screen_points, float world_z = 0.0f) const;

    /**
     * @brief Projects the screen's point to the camera plane
     * @param screen_x - the screen's point x coordinate
     * @param screen_y - the screen's point y coordinate
     * @param distance - the relative value of camera plane (0 coresponds to near plane, 1 coresponds to far plane)
     * @return Point in world coordinates
     */
    gl_scene::Vec3 toWorldCoordinates(int screen_x, int screen_y, float distance) const;

    /**
     * @brief Projects the screen's point to the camera plane
     * @param screen_points - the container with screen's points
     * @param distance - the relative value of camera plane (0 coresponds to near plane, 1 coresponds to far plane)
     * @return Points in world coordinates
     */
    gl_scene::Point3Pack toWorldCoordinates(const gl_scene::Point2Pack& screen_points, float distance) const;

    /**
     * @brief Projects the world's point to the screen
     * @param world_point - the world's point coordinates
     * @return Point's screen coordinates
     */
    gl_scene::Vec2 toScreenCoordinates(const gl_scene::Vec3& world_point) const;

    /** setters */
    void resetTo(const Camera& camera);
    void setPitch(float pitch);
    void setYaw(float yaw);
    void setZoom(float zoom);
    void setScale(const Vec3& scale);
    void setPosition(const Vec3& position);
    inline void setSeneitivity(float sensetivity) { mSensitivity = sensetivity; }
    inline void setSpeed(float speed) { mSpeed = speed; }
    void setProjectionType(Projection::Type proj_type);
    void setViewPort(int view_width, int view_height);
    void setPerspective(const ProjectionPerspective& perspective);
    void setOrtho(const ProjectionOrtho& ortho);

    /** getters */
    inline Mat4 getTransformation() const { return getProjection() * getView(); }
    inline const Mat4& getView() const { return mViewMatrix; }
    inline const Mat4& getProjection() const { return mCurrentProjection->get(); }
    inline float getYaw() const { return mYaw; }
    inline float getPitch() const { return mPitch; }
    inline float getZoom() const { return mZoom; }
    inline const Vec3& getScale() { return mScale; }
    inline const Vec3& getPosition() const { return mPosition; }
    inline const Vec3& getRight() const { return mRight; }
    inline const Vec3& getUp() const { return mUp; }
    inline const Vec3& getFront() const { return mFront; }
    inline const Vec3& getLook() const { return mLook; }
    inline const Vec3& getLookPoint() const { return mLookPoint; }
    inline Projection::Type getProjectionType() const { return mProjectionType; }
    inline const ProjectionPerspective& perspective() const { return mProjectionPerspective; }
    inline const ProjectionOrtho& ortho() const { return mProjectionOrtho; }
    inline const RangeLimits& getRangeLimits() const { return mRangeLimits; }
    inline const std::pair<int, int>& getViewPortSize() const { return mViewPortSize; }

 signals:
    void signalChanged(const gl_scene::Mat4& transformation);

 private:
    void update(bool use_angles = true, bool update_look = false);
    void calculateViewMatrix();
    void checkRangeLimits();

    float mYaw;
    float mPitch;
    float mSpeed;
    float mSensitivity;
    float mZoom;
    Vec3 mScale;
    Vec3 mPosition;
    Vec3 mFront;
    Vec3 mXYFront;
    Vec3 mUp;
    Vec3 mRight;
    Vec3 mWorldUp;
    Vec3 mLookPoint;
    Vec3 mLook;
    RangeLimits mRangeLimits;
    std::pair<int, int> mViewPortSize;
    Projection::Type mProjectionType;
    ProjectionPerspective mProjectionPerspective;
    ProjectionOrtho mProjectionOrtho;
    Projection* mCurrentProjection;
    Mat4 mViewMatrix;
};

}  // namespace gl_scene
