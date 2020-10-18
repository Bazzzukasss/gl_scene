#pragma once

#include "gl_scene_types.h"

namespace gl_scene
{
/**
 * The Projection Class
 * @brief The class for projections of the camera.
 * It is used for calculation of view during the scene rendering.
 */
class Projection
{
 public:
    enum class Type
    {
        kOrtho,
        kPerspective
    };

    /**
     * @brief The coonstructor for the Projection
     * @param type - the projection type
     * @param ratio - the screen's viewport ratio
     */
    Projection(Type type, float ratio);
    virtual ~Projection() = default;

    /* setters */
    virtual void setRatio(float ratio) = 0;

    /* getters */
    float getRatio() const { return mRatio; }
    inline const Mat4& get() const { return mProjection; }
    virtual float getProjectionKoef(float camera_position, int view_width) const = 0;

 protected:
    Projection::Type mType;
    Mat4 mProjection;
    float mRatio;
};

/**
 * The ProjectionOrtho Class
 * @brief The class represents orthogonal view projection
 */
class ProjectionOrtho : public Projection
{
 public:
    /**
     * @brief Constructor for ProjectionOrtho
     * @param min_x - minimal value by x axis
     * @param max_x - maximal value by x axis
     * @param min_y - minimal value by y axis
     * @param max_y - maximal value by y axis
     * @param min_z - minimal value by z axis
     * @param max_z - maximal value by z axis
     * @param ratio - screen viewport ratio
     */
    ProjectionOrtho(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, float ratio,
                    const Vec3& scale = {1.0f, 1.0f, 1.0f});

    /** setters */
    void setRatio(float ratio) override;
    void setXRange(float min, float max);
    void setYRange(float min, float max);
    void setZRange(float min, float max);
    void setScale(const Vec3& scale);

    /** getters */
    float getProjectionKoef(float camera_position, int view_width) const override;
    float getProjectedX(float x, int view_width) const;
    inline std::pair<float, float> getXRange() const { return {mMinX, mMaxX}; }
    inline std::pair<float, float> getYRange() const { return {mMinY, mMaxY}; }
    inline std::pair<float, float> getZRange() const { return {mMinZ, mMaxZ}; }

 private:
    void calculate();

    float mMinX;
    float mMaxX;
    float mMinY;
    float mMaxY;
    float mMinZ;
    float mMaxZ;
    Vec3 mScale;
};

/**
 * The ProjectionPerspective Class
 * @brief The class represents perspective view projection
 */
class ProjectionPerspective : public Projection
{
 public:
    /**
     * @brief The constructor for the ProjectionPerspective
     * @param alfa - the angle of the projection
     * @param near_distance - the distance to the near projection plane
     * @param far_distance - the distance to the far projection plane
     * @param ratio - the screen's viewport ratio
     */
    ProjectionPerspective(float alfa, float near_distance, float far_distance, float ratio);

    /** setters */
    void setRatio(float ratio) override;
    void setAlfa(float alfa);
    void setNear(float near);
    void setFar(float far);

    /** getters */
    float getProjectionKoef(float camera_position, int view_width) const override;

 private:
    void calculate();

    float mAlfa;
    float mNear;
    float mFar;
};

}  // namespace gl_scene
