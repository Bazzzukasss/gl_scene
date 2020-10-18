#pragma once

#include "gl_scene_types.h"

#define GENERATE gl_scene::Generator::generate
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace gl_scene
{

/**
 * The Generator Class
 * @brief The class provides functionality for generation of various figures' geometry
 */
class Generator
{
 public:
    Generator();

    /**
     * @brief The set of functions for geometry generation
     * @param figure - the data structure containing the figure's geometry
     * @return the container with points
     */
    static Point3Pack generate(const FigureStripe& figure);
    static Point3Pack generate(const FigureTube& figure);
    static Point3Pack generate(const FigureCube& figure);
    static Point3Pack generate(const FigureSphere& figure);
    static Point3Pack generate(const FigureCylinder& figure);
    static Point3Pack generate(const FigureConus& figure);
    static Point3Pack generate(const FigureGrid& figure);
    static Point3Pack generate(const FigureExtrusion& figure);
    static Point3Pack generate(const FigurePath& figure);
    static Point3Pack generate(const FigureFOV& figure);
    static Point3Pack generate(const FigureWireBox& figure);

 private:
    static inline void mergePoints(const Point3Pack& from, Point3Pack& to);
    static Point3Pack genQuad(const Point3& p1, const Point3& p2, const Point3& p3, const Point3& p4);
    static Point3Pack genTriangle(const Point3& p1, const Point3& p2, const Point3& p3);
    static Point3Pack genSide(const Point3& center_point, const Point3Pack& circle_points, bool is_ccw = true);
    static Point3Pack genCircle(float radius, uint sides, float z, float start_angle = 0);
    static Point3Pack genSphere(float radius, uint sides_h, uint sides_v, float z);
    static bool isEqual(const Point3& p1, const Point3& p2);
};

void Generator::mergePoints(const Point3Pack& from, Point3Pack& to)
{
    std::copy(from.begin(), from.end(), std::back_inserter(to));
}

}  // namespace gl_scene
