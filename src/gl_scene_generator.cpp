#include "gl_scene_generator.h"
#include "gl_scene_utility.h"
#include <QtMath>
#include <QPainterPath>

using namespace gl_scene;

Generator::Generator()
{}

Point3Pack Generator::generate(const FigureStripe& figure)
{
    Point3Pack points;
    if (!figure.upperShape.empty())
    {
        const auto& sides = figure.upperShape.size();
        for (uint i{1}; i < sides; ++i)
        {
            const auto& p1 = figure.upperShape[i - 1];
            const auto& p2 = figure.upperShape[i];
            const auto& p3 = figure.lowerShape[i];
            const auto& p4 = figure.lowerShape[i - 1];

            mergePoints(genQuad(p1, p2, p3, p4), points);
        }

        const auto& p1 = figure.upperShape[sides - 1];
        const auto& p2 = figure.upperShape[0];
        const auto& p3 = figure.lowerShape[0];
        const auto& p4 = figure.lowerShape[sides - 1];

        mergePoints(genQuad(p1, p2, p3, p4), points);
    }

    return points;
}

Point3Pack Generator::generate(const FigureTube& figure)
{
    Point3Pack points;
    Point3Pack topPoints;
    Point3Pack bottomPoints;

    const auto& sides = figure.shape.size();
    for (uint i{0}; i < sides; ++i)
    {
        const auto& p = figure.shape[i];
        topPoints.push_back({p[0], p[1], figure.height / 2.0f});
        bottomPoints.push_back({p[0], p[1], -figure.height / 2.0f});
    }

    points = generate(FigureStripe{topPoints, bottomPoints});
    mergePoints(genSide({0.0f, 0.0f, figure.height / 2.0f}, topPoints), points);
    mergePoints(genSide({0.0f, 0.0f, -figure.height / 2.0f}, bottomPoints), points);

    return points;
}

Point3Pack Generator::generate(const FigureCube& figure)
{
    return generate(FigureCylinder{sqrtf(2.0f * figure.size * figure.size) / 2.0f, figure.size, 4, 45.0});
}

Point3Pack Generator::generate(const FigureSphere& figure)
{
    Point3Pack points;
    Point3Pack spherePoints{genSphere(figure.radius, figure.hSides, figure.vSides, -figure.radius)};
    for (uint v{0}; v < figure.vSides; ++v)
    {
        for (uint h{0}; h < figure.hSides; ++h)
        {
            uint ti        = v * (figure.hSides + 1) + h;
            uint bi        = (v + 1) * (figure.hSides + 1) + h;
            const auto& p1 = spherePoints[ti];
            const auto& p2 = spherePoints[ti + 1];
            const auto& p3 = spherePoints[bi + 1];
            const auto& p4 = spherePoints[bi];
            mergePoints(genQuad(p1, p2, p3, p4), points);
        }
    }

    return points;
}

Point3Pack Generator::generate(const FigureCylinder& figure)
{
    Point3Pack points;
    Point3 topPoint{0.0f, 0.0f, figure.height / 2.0f};
    Point3 bottomPoint{0.0f, 0.0f, -figure.height / 2.0f};
    const auto& topCirclePoints{genCircle(figure.radius, figure.sides, topPoint[2], figure.angle)};
    const auto& bottomCirclePoints{genCircle(figure.radius, figure.sides, bottomPoint[2], figure.angle)};

    points = generate(FigureStripe{topCirclePoints, bottomCirclePoints});

    mergePoints(genSide(topPoint, topCirclePoints), points);
    mergePoints(genSide(bottomPoint, bottomCirclePoints, false), points);

    return points;
}

Point3Pack Generator::generate(const FigureConus& figure)
{
    Point3Pack points;
    const auto& circlePoints{genCircle(figure.radius, figure.sides, -figure.height / 2.0f)};

    const Point3& topPoint{0.0f, 0.0f, figure.height / 2.0f};
    const auto& topPoints{genSide(topPoint, circlePoints)};

    const Point3& bottomPoint{0.0f, 0.0f, -figure.height / 2.0f};
    const auto& bottomPoints{genSide(bottomPoint, circlePoints, false)};

    mergePoints(topPoints, points);
    mergePoints(bottomPoints, points);

    return points;
}

Point3Pack Generator::generate(const FigureGrid& figure)
{
    Point3Pack points;

    for (float x = 0; x < figure.maxX; x += figure.cellSizeX)
    {
        points.push_back({x, figure.minY, 0.0f});
        points.push_back({x, figure.maxY, 0.0f});
    }

    for (float x = 0; x >= figure.minX; x -= figure.cellSizeX)
    {
        points.push_back({x, figure.minY, 0.0f});
        points.push_back({x, figure.maxY, 0.0f});
    }

    for (float y = 0; y < figure.maxY; y += figure.cellSizeY)
    {
        points.push_back({figure.minX, y, 0.0f});
        points.push_back({figure.maxX, y, 0.0f});
    }

    for (float y = 0; y >= figure.minY; y -= figure.cellSizeY)
    {
        points.push_back({figure.minX, y, 0.0f});
        points.push_back({figure.maxX, y, 0.0f});
    }

    points.push_back({figure.maxX, figure.minY, 0.0f});
    points.push_back({figure.maxX, figure.maxY, 0.0f});
    points.push_back({figure.minX, figure.minY, 0.0f});
    points.push_back({figure.minX, figure.maxY, 0.0f});

    points.push_back({figure.maxX, figure.maxY, 0.0f});
    points.push_back({figure.minX, figure.maxY, 0.0f});
    points.push_back({figure.maxX, figure.minY, 0.0f});
    points.push_back({figure.minX, figure.minY, 0.0f});

    return points;
}

Point3Pack Generator::generate(const FigureExtrusion& figure)
{
    if (figure.shape.size() == 2)
    {
        const auto& width = fabs(static_cast<double>(figure.shape[0][0] - figure.shape[1][0]));
        return generate(FigurePath{figure.path, static_cast<float>(width)});
    }

    return figure.path;
}

Point3Pack Generator::generate(const FigurePath& figure)
{
    Point3Pack points;
    std::vector<std::pair<Point3, Point3>> pathPoints;
    for (int i{0}; i < static_cast<int>(figure.path.size()) - 1; ++i)
    {
        const auto& p1 = figure.path[static_cast<uint>(i)];
        const auto& p2 = figure.path[static_cast<uint>(i) + 1];
        Point3 qp1, qp2;

        qp1 = Point3{p1[0] + figure.width / 2, p1[1], p1[2]};
        qp2 = Point3{p1[0] - figure.width / 2, p1[1], p1[2]};

        pathPoints.push_back({qp1, qp2});

        if (i == static_cast<int>(figure.path.size()) - 2)
        {
            qp1 = Point3{p2[0] + figure.width / 2, p2[1], p2[2]};
            qp2 = Point3{p2[0] - figure.width / 2, p2[1], p2[2]};
        }
        pathPoints.push_back({qp1, qp2});
    }

    for (int i = 0; i < static_cast<int>(pathPoints.size()) - 1; i++)
    {
        const auto& p1 = pathPoints[static_cast<uint>(i)];
        const auto& p2 = pathPoints[static_cast<uint>(i) + 1];

        const auto& qp1 = p1.first;
        const auto& qp2 = p2.first;
        const auto& qp3 = p2.second;
        const auto& qp4 = p1.second;

        mergePoints(genQuad(qp1, qp2, qp3, qp4), points);
    }

    return points;
}

Point3Pack Generator::generate(const FigureFOV& figure)
{
    Point3Pack points;

    qreal mainFovStartAngle      = -figure.mountingAngle + figure.primaryFov / 2.0 - 90;
    qreal secondaryFovStartAngle = -figure.mountingAngle + figure.secondaryFov / 2.0 - 90;

    QRectF mainFovRect(-figure.primaryRange, -figure.primaryRange, figure.primaryRange * 2, figure.primaryRange * 2);
    QRectF secondaryFovRect(-figure.secondaryRange, -figure.secondaryRange, figure.secondaryRange * 2,
                            figure.secondaryRange * 2);

    QPainterPath path;
    path.lineTo(figure.secondaryRange * qSin(M_PI_2 + qDegreesToRadians(secondaryFovStartAngle)),
                figure.secondaryRange * qCos(M_PI_2 + qDegreesToRadians(secondaryFovStartAngle)));
    path.arcTo(secondaryFovRect, secondaryFovStartAngle, -(figure.secondaryFov - figure.primaryFov) / 2);
    path.lineTo(figure.primaryRange * qSin(M_PI_2 + qDegreesToRadians(mainFovStartAngle)),
                figure.primaryRange * qCos(M_PI_2 + qDegreesToRadians(mainFovStartAngle)));
    path.arcTo(mainFovRect, mainFovStartAngle, -figure.primaryFov);
    path.lineTo(figure.secondaryRange * qSin(M_PI_2 + qDegreesToRadians(mainFovStartAngle - figure.primaryFov)),
                figure.secondaryRange * qCos(M_PI_2 + qDegreesToRadians(mainFovStartAngle - figure.primaryFov)));
    path.arcTo(secondaryFovRect, mainFovStartAngle - figure.primaryFov, -(figure.secondaryFov - figure.primaryFov) / 2);
    path.lineTo(0, 0);

    path.translate(-figure.mountingPoint.x(), figure.mountingPoint.y());
    for (const auto& p : path.toFillPolygon())
    {
        points.push_back({static_cast<float>(p.x()), static_cast<float>(p.y()), 0.0f});
    }

    return points;
}

Point3Pack Generator::generate(const FigureWireBox& figure)
{
    float l{figure.size};
    float h{figure.size};
    float w{figure.size};

    Point3Pack points{
        {-w / 2, -l / 2, -h / 2}, {-w / 2, l / 2, -h / 2}, {-w / 2, l / 2, -h / 2}, {w / 2, l / 2, -h / 2},
        {w / 2, l / 2, -h / 2},   {w / 2, -l / 2, -h / 2}, {w / 2, -l / 2, -h / 2}, {-w / 2, -l / 2, -h / 2},

        {-w / 2, -l / 2, h / 2},  {-w / 2, l / 2, h / 2},  {-w / 2, l / 2, h / 2},  {w / 2, l / 2, h / 2},
        {w / 2, l / 2, h / 2},    {w / 2, -l / 2, h / 2},  {w / 2, -l / 2, h / 2},  {-w / 2, -l / 2, h / 2},

        {-w / 2, -l / 2, -h / 2}, {-w / 2, -l / 2, h / 2}, {-w / 2, l / 2, -h / 2}, {-w / 2, l / 2, h / 2},
        {w / 2, l / 2, -h / 2},   {w / 2, l / 2, h / 2},   {w / 2, -l / 2, -h / 2}, {w / 2, -l / 2, h / 2},
    };

    if (figure.isCrossed)
    {
        Point3Pack crossPoints{
            {-w / 2, -l / 2, -h / 2}, {w / 2, l / 2, h / 2},  {w / 2, -l / 2, -h / 2}, {-w / 2, l / 2, h / 2},

            {-w / 2, l / 2, -h / 2},  {w / 2, -l / 2, h / 2}, {w / 2, l / 2, -h / 2},  {-w / 2, -l / 2, h / 2}, {}, {}};
        mergePoints(crossPoints, points);
    }

    return points;
}

Point3Pack Generator::genQuad(const Point3& p1, const Point3& p2, const Point3& p3, const Point3& p4)
{
    Point3Pack points;
    const auto& triangle1Points = genTriangle(p1, p2, p3);
    const auto& triangle2Points = genTriangle(p3, p4, p1);

    mergePoints(triangle1Points, points);
    mergePoints(triangle2Points, points);

    return points;
}

Point3Pack Generator::genTriangle(const Point3& p1, const Point3& p2, const Point3& p3)
{
    Point3Pack points;
    if (!isEqual(p1, p2) && !isEqual(p2, p3) && !isEqual(p3, p1))
    {
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
    }
    return points;
}

Point3Pack Generator::genSide(const Point3& center_point, const Point3Pack& side_points, bool is_ccw)
{
    Point3Pack points;
    if (!side_points.empty())
    {
        for (uint s{1}; s < side_points.size(); ++s)
        {
            const auto& p1 = side_points[s - 1];
            const auto& p2 = side_points[s];
            if (is_ccw)
            {
                mergePoints(genTriangle(center_point, p2, p1), points);
            }
            else
            {
                mergePoints(genTriangle(center_point, p1, p2), points);
            }
        }

        const auto& p1 = side_points[side_points.size() - 1];
        const auto& p2 = side_points[0];
        if (is_ccw)
        {
            mergePoints(genTriangle(center_point, p2, p1), points);
        }
        else
        {
            mergePoints(genTriangle(center_point, p1, p2), points);
        }
    }
    return points;
}

Point3Pack Generator::genCircle(float radius, uint sides, float z, float start_angle)
{
    Point3Pack points;
    for (uint s{0}; s <= sides; ++s)
    {
        float angle = s * 2 * static_cast<float>(M_PI) / static_cast<float>(sides) + qDegreesToRadians(start_angle);
        float x     = radius * sinf(angle);
        float y     = radius * cosf(angle);
        points.push_back({x, y, z});
    }

    return points;
}

Point3Pack Generator::genSphere(float radius, uint sides_h, uint sides_v, float z)
{
    Point3Pack points;
    for (float v{0}; v <= sides_v; ++v)
    {
        float angleV             = v * static_cast<float>(M_PI) / static_cast<float>(sides_v);
        float h                  = radius + radius * cosf(angleV);
        float r                  = radius * sinf(angleV);
        const auto& circlePoints = genCircle(r, sides_h, z + h);
        mergePoints(circlePoints, points);
    }

    return points;
}

bool Generator::isEqual(const Point3& p1, const Point3& p2)
{
    return qFuzzyIsNull(p1[0] - p2[0]) && qFuzzyIsNull(p1[1] - p2[1]) && qFuzzyIsNull(p1[2] - p2[2]);
}
