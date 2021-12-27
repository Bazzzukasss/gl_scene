#pragma once

#include "gl_scene_types.h"

namespace gl_scene
{

extern Point3 toPoint3(const Vec3& vec);

/**
 * @brief Creates container with color's components
 * @param color - the value of the color
 * @return the container with the color's components
 */
extern Vec3 toVec3(const Color& color);

/**
 * @brief Finds the cross point of the two line segments within the OXY plane
 * @param a1 - the first segment's begin point
 * @param a1 - the first segment's end point
 * @param a1 - the second segment's begin point
 * @param a1 - the second segment's end point
 * @return The container with cross point. If segments have no cross point the container is empty.
 */
extern Point3Pack crossPoint(const Point3& a1, const Point3& a2, const Point3& b1, const Point3& b2);

/**
 * @brief Finds cross point of the segment and the figure within the OXY plane
 * @param p1 - the segment's begin point
 * @param p1 - the segment's end point
 * @param figure - the container with the figure's points
 * @return The container with cross points. If segment has no cross point with the figure the container is empty.
 */
extern Point3Pack crossPoints(const Point3& p1, const Point3& p2, const Point3Pack& figure);

}  // namespace gl_scene
