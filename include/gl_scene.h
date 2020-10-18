#pragma once

#include "gl_scene_mesh.h"
#include "gl_scene_defaults.h"
#include "gl_scene_item.h"
#include "gl_scene_object.h"

namespace gl_scene
{

/**
 * The Scene Class
 * @brief The class represents 3D world scene.
 * It contains all necessary data for visualization of the 3D world.
 * It provides all routinas to manipulate scene's items and objects.
 */
class Scene
{
 public:
    using Ptr = std::shared_ptr<Scene>;
    /**
     * @brief Constructor for the Scene. Creates Scene with all necessary data.
     * @param meshes - the container with meshes (is necessary for visuzlization of the scene's items with static
     * geometry)
     * @param light - light parameters (OpenGL pipeline uses it for scene rendering)
     * @param shaders - the container with shaders for the OpenGL pipeline
     */
    Scene(const Mesh::Map& meshes_map = defaults::meshes::kDefault, const Light& light = defaults::lights::kDefault,
          const Shader::Map& shader_map   = defaults::shaders::kDefault,
          const TexturesMap& textures_map = defaults::textures::kDefault);
    virtual ~Scene() = default;

    /**
     * @brief Adds item into the scene
     * @param item - the scene item record
     * @return shared pointer to added item
     */
    Item::Ptr addItem(const Item& item);

    /**
     * @brief Adds item into the scene
     * @param item_ptr - the shared pointer to the added item
     */
    void addItem(const Item::Ptr& item_ptr);

    /**
     * @brief Adds the object wich into the scene
     * @param obj_ptr - the shared pointer to the added object
     */
    void addObject(const SceneObject::Ptr& obj_ptr);

    /**
     * @brief Updates all objects in the scene
     */
    void update();

    /** setters */
    inline void setTextItems(const gl_scene::TextItem::Pack& items) { mTextItemPack = items; }

    /** getters */
    inline const gl_scene::TextItem::Pack& getTextItems() const { return mTextItemPack; }
    inline const VertexPack& getVertices() const { return mVertices; }
    inline const Shader::Map& getShaders() const { return mShaderMap; }
    inline const Item::PtrMap& getItems() const { return mItemPtrMap; }
    inline const Light& getLight() const { return mLight; }
    GeometryData getGeometryData(MeshID mesh_id) const;
    Texture::Ptr getTexture(TextureID texture_id);

 protected:
    SceneObject::PtrPack mObjectPtrPack;

 private:
    void initialize();

    const Mesh::Map& mMeshMap;
    Light mLight;
    const Shader::Map& mShaderMap;
    VertexPack mVertices;
    Mesh::GeometryMap mMeshGeometryMap;
    Item::PtrMap mItemPtrMap;
    TextItem::Pack mTextItemPack;
    TexturesMap mTexturesMap;
};

}  // namespace gl_scene
