#pragma once

#include "gl_scene_types.h"

namespace gl_scene
{

/**
 * The Mesh Class
 * @brief The class converts the geometry (the set of points) to the mesh (the set of vertices).
 * Meshes is used for rendering of the scene's items.
 */
class Mesh
{
 public:
    using Map         = std::map<MeshID, Mesh>;
    using GeometryMap = std::map<MeshID, GeometryData>;
    /**
     * @brief Constructor for Mesh
     * @param vertices - the container with model vertices
     */
    explicit Mesh(const VertexPack& vertex_pack);

    /**
     * @brief Constructor for Mesh
     * @param point_pack - the container with model points
     * @param z - the value of the z axis shift
     * @param is_generate_normals - the normals' generation flag (if true the normals will be generated for each
     * vertex). The normals must be generated if the mesh is supposed to be rendered by the OpenGL pipeline with the
     * scene's light. WARNING: 3 mesh points are needed to generate one normal (so that a poligon can be calculated). It
     * means the amount of points must be the factor of 3, otherwise the last one or two vertices will contan zero
     * normal. If mesh is supposed to be rendered by the OpenGL pipeline without the scene's light (just the item's
     * color will be used) the normal genegation can be skipped.
     */
    explicit Mesh(const Point3Pack& point_pack, float z = 0.0f, bool is_generate_normals = true);

    /**
     * @brief Constructor for Mesh
     * @param point_pack - the container with model points
     * @param is_generate_normals - the normals' generation flag (if true the normals will be generated for each
     * vertex). The normals must be generated if the mesh is supposed to be rendered by the OpenGL pipeline with the
     * scene's light. WARNING: 3 mesh points are needed to generate one normal (so that a poligon can be calculated). It
     * means the amount of points must be the factor of 3, otherwise the last one or two vertices will contan zero
     * normal. If mesh is supposed to be rendered by the OpenGL pipeline without the scene's light (just the item's
     * color will be used) the normal genegation can be skipped.
     */
    explicit Mesh(const Point3Pack& point_pack, bool is_generate_normals);

    /** getters */
    inline const VertexPack& getVertices() const { return mVertexPack; }
    inline const Point3Pack& getPoints() const { return mPointPack; }

 private:
    void levelPoints(float z);
    void generateVertices(bool is_generate_normals);
    QVector3D calculateNormal(const Point3& v1, const Point3& v2, const Point3& v3) const;

    Point3Pack mPointPack;
    VertexPack mVertexPack;
};

}  // namespace gl_scene
