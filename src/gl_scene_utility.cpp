#include "gl_scene_utility.h"

using namespace gl_scene;

Vec3 gl_scene::toVec3(const Color& color)
{
    return {static_cast<float>(color.redF()), static_cast<float>(color.greenF()), static_cast<float>(color.blueF())};
}

Point3Pack gl_scene::crossPoint(const Point3& a1, const Point3& a2, const Point3& b1, const Point3& b2)
{
    const auto& x1 = a1[0] < a2[0] ? a1[0] : a2[0];
    const auto& x2 = a1[0] < a2[0] ? a2[0] : a1[0];
    const auto& y1 = a1[0] < a2[0] ? a1[1] : a2[1];
    const auto& y2 = a1[0] < a2[0] ? a2[1] : a1[1];

    const auto& x3 = b1[0] < b2[0] ? b1[0] : b2[0];
    const auto& x4 = b1[0] < b2[0] ? b2[0] : b1[0];
    const auto& y3 = b1[0] < b2[0] ? b1[1] : b2[1];
    const auto& y4 = b1[0] < b2[0] ? b2[1] : b1[1];

    const auto& minX12 = std::min(x1, x2);
    const auto& minX34 = std::min(x3, x4);
    const auto& minY12 = std::min(y1, y2);
    const auto& minY34 = std::min(y3, y4);

    const auto& maxX12 = std::max(x1, x2);
    const auto& maxX34 = std::max(x3, x4);
    const auto& maxY12 = std::max(y1, y2);
    const auto& maxY34 = std::max(y3, y4);

    auto isInside = [&](float x, float y) {
        return ((minX34 <= x) && (x <= maxX34) && (minX12 <= x) && (x <= maxX12) && (minY34 <= y) && (y <= maxY34) &&
                (minY12 <= y) && (y <= maxY12));
    };

    if (!qFuzzyIsNull(x2 - x1) && !qFuzzyIsNull(x3 - x4))
    {
        const auto& k1 = (y2 - y1) / (x2 - x1);
        const auto& k2 = (y4 - y3) / (x4 - x3);

        if (!qFuzzyIsNull(k1 - k2))
        {
            const auto& c1 = y1 - k1 * x1;
            const auto& c2 = y3 - k2 * x3;

            const auto& x = (c2 - c1) / (k1 - k2);
            const auto& y = k1 * x + c1;

            if (isInside(x, y))
            {
                return {{x, y, 0.0f}};
            }
        }
    }

    if (qFuzzyIsNull(x2 - x1) && !qFuzzyIsNull(x3 - x4))
    {
        const auto& k = (y4 - y3) / (x4 - x3);
        const auto& c = y3 - k * x3;

        const auto& x = x1;
        const auto& y = k * x + c;
        if (isInside(x, y))
        {
            return {{x, y, 0.0f}};
        }
    }

    if (!qFuzzyIsNull(x2 - x1) && qFuzzyIsNull(x3 - x4))
    {
        const auto& k = (y2 - y1) / (x2 - x1);
        const auto& c = y1 - k * x1;

        const auto& x = x3;
        const auto& y = k * x + c;

        if (isInside(x, y))
        {
            return {{x, y, 0.0f}};
        }
    }

    return {};
}

Point3Pack gl_scene::crossPoints(const Point3& p1, const Point3& p2, const Point3Pack& figure)
{
    Point3Pack points;
    for (uint i = 0; i < figure.size() - 1; ++i)
    {
        const auto& crossPoints = crossPoint(p1, p2, figure[i], figure[i + 1]);
        if (!crossPoints.empty())
        {
            std::copy(crossPoints.begin(), crossPoints.end(), std::back_inserter(points));
        }
    }

    return points;
}

Point3 gl_scene::toPoint3(const Vec3& vec)
{
    return {vec.x(), vec.y(), vec.z()};
}
