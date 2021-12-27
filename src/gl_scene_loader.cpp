#include "gl_scene_loader.h"
#include <vector>
#include <QFileInfo>

using namespace gl_scene;

gl_scene::VertexPack gl_scene::ModelLoader::load(const std::string& filename)
{
    QFileInfo info(QString::fromStdString(filename));
    if (info.exists())
    {
        if (info.completeSuffix() == "obj")
        {
            return loadObj(filename);
        }
    }

    return {};
}

gl_scene::VertexPack gl_scene::ModelLoader::loadObj(const std::string& filename)
{
    std::vector<int> vertexIndices, uvIndices, normalIndices;
    std::vector<Vec3> temp_vertices;
    std::vector<Vec2> temp_uvs;
    std::vector<Vec3> temp_normals;

    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return {};
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line      = in.readLine();
        auto lineArgs     = line.split(" ");
        auto lineArgsSize = lineArgs.size();

        if (lineArgs.empty())
        {
            file.close();
            return {};
        }

        QString lineHeader = lineArgs[0];
        if (lineHeader == "v")
        {
            Vec3 vertex;
            if (lineArgsSize >= 4)
            {
                vertex[0] = lineArgs[1].toFloat();
                vertex[1] = lineArgs[2].toFloat();
                vertex[2] = lineArgs[3].toFloat();
                temp_vertices.push_back(vertex);
            }
        }
        else if (lineHeader == "vt")
        {
            Vec2 uv;
            if (lineArgsSize >= 3)
            {
                uv[0] = lineArgs[1].toFloat();
                uv[1] = -lineArgs[1].toFloat();
                temp_uvs.push_back(uv);
            }
        }
        else if (lineHeader == "vn")
        {
            Vec3 normal;
            if (lineArgsSize >= 4)
            {
                normal[0] = lineArgs[1].toFloat();
                normal[1] = lineArgs[2].toFloat();
                normal[2] = lineArgs[3].toFloat();
                temp_normals.push_back(normal);
            }
        }
        else if (lineHeader == "f")
        {
            int vertexIndex[3]{}, uvIndex[3]{}, normalIndex[3]{};

            if (lineArgsSize == 4)
            {
                auto vertexIndexList1 = lineArgs[1].split("/");
                auto vertexIndexList2 = lineArgs[2].split("/");
                auto vertexIndexList3 = lineArgs[3].split("/");
                if ((vertexIndexList1.size() == 3) && (vertexIndexList2.size() == 3) && (vertexIndexList3.size() == 3))
                {
                    vertexIndex[0] = vertexIndexList1[0].toInt();
                    uvIndex[0]     = vertexIndexList1[1].toInt();
                    normalIndex[0] = vertexIndexList1[2].toInt();

                    vertexIndex[1] = vertexIndexList2[0].toInt();
                    uvIndex[1]     = vertexIndexList2[1].toInt();
                    normalIndex[1] = vertexIndexList2[2].toInt();

                    vertexIndex[2] = vertexIndexList3[0].toInt();
                    uvIndex[2]     = vertexIndexList3[1].toInt();
                    normalIndex[2] = vertexIndexList3[2].toInt();
                }
                else
                {
                    qInfo() << "File can't be read by our simple parser :-( Try exporting with other options\n";
                    file.close();
                    return {};
                }
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }
    file.close();

    VertexPack vertices;
    // For each vertex of each triangle
    for (uint i = 0; i < vertexIndices.size(); i++)
    {
        // Get the indices of its attributes
        const auto& vertexIndex = vertexIndices[i];
        // unsigned int uvIndex = uvIndices[i];
        const auto& normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        Vec3 vertex = temp_vertices[static_cast<uint>(vertexIndex - 1)];
        // Vec2 uv = temp_uvs[ uvIndex-1 ];
        Vec3 normal = temp_normals[static_cast<uint>(normalIndex - 1)];

        vertices.push_back({vertex[0], vertex[1], vertex[2], normal[0], normal[1], normal[2]});
    }

    return vertices;
}
