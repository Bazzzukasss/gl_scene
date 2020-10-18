#define _USE_MATH_DEFINES
#include <cmath>

#include "gl_scene_mesh.h"

using namespace gl_scene;

Mesh::Mesh(const VertexPack& vertex_pack) : mVertexPack(vertex_pack)
{}

Mesh::Mesh(const Point3Pack& point_pack, float z, bool is_generate_normals) : mPointPack(point_pack)
{
    if (z != 0.0f)
    {
        levelPoints(z);
    }

    generateVertices(is_generate_normals);
}

Mesh::Mesh(const Point3Pack& points, bool is_generate_normals) : Mesh(points, 0.0f, is_generate_normals)
{}

void Mesh::levelPoints(float z)
{
    for (auto& p : mPointPack)
    {
        p[2] += z;
    }
}

void Mesh::generateVertices(bool is_generate_normals)
{
    if (is_generate_normals)
    {
        for (uint pol = 0; pol < mPointPack.size() / 3; ++pol)
        {
            uint p             = pol * 3;
            const auto& normal = calculateNormal(mPointPack[p], mPointPack[p + 1], mPointPack[p + 2]);

            for (uint i = 0; i < 3; ++i)
            {
                mVertexPack.push_back({mPointPack[p + i][0], mPointPack[p + i][1], mPointPack[p + i][2], normal.x(),
                                       normal.y(), normal.z()});
            }
        }
    }
    else
    {
        for (uint p = 0; p < mPointPack.size(); p++)
        {
            mVertexPack.push_back({mPointPack[p][0], mPointPack[p][1], mPointPack[p][2], 0.0f, 0.0f, 1.0f});
        }
    }
}

QVector3D Mesh::calculateNormal(const Point3& v1, const Point3& v2, const Point3& v3) const
{
    Vec3 edge1{v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2]};
    Vec3 edge2{v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2]};

    return QVector3D::crossProduct(edge1, edge2);
}
