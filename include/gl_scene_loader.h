#pragma once

#include "gl_scene_types.h"

#define LOAD ModelLoader::load

namespace gl_scene
{

/**
 * The ModelLoader Class
 * @brief The class provides functionality to load the 3D models' geometry from files
 */
class ModelLoader
{
 public:
    ModelLoader() = default;

    /**
     * @brief Loads the mesh from the file
     * @param filename - the 3D model file name
     * @return the container with vertices
     */
    static VertexPack load(const std::string& filename);

 private:
    static VertexPack loadObj(const std::string& filename);
};
}  // namespace gl_scene
