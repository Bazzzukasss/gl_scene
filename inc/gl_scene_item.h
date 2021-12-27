#pragma once

#include "gl_scene_types.h"
#include "gl_scene_defaults.h"

namespace gl_scene
{
/**
 * The Item Strusture
 * @brief The structure contains data for rendering of graphics element on the scene
 */

struct Item
{
    using Pack    = std::vector<Item>;
    using Ptr     = std::shared_ptr<Item>;
    using PtrPack = std::vector<Item::Ptr>;
    using PtrMap  = std::map<PipeID, Item::PtrPack>;
    using IdPack  = std::set<ItemID>;

    /**
     * @brief Constructor for Item
     * @param is_visible - the value of the item's visibility
     * @param mesh_id - the ID of the scene item mesh
     * @param vertex_pack - the container with an item's vertices (is used for rendering in case the item has mutable
     * geometry)
     * @param item_color - the item's material color
     * @param is_mutable - the item's geometry type (if true then item_vertices will be used for rendering, otherwise
     * geom_data structure will be used)
     * @param item_id - the ID of an item. It will be taken into account while selecting and hovering items.
     * If equal to 0 then item could not be selected or hovered.
     * (WARNING: only first 24 bits metter!)
     * @param render_parameters - the data structure for the OpenGL pipeline. These parameters will be used for item
     * rendering.
     * @param pipe_id - the ID of the predefined pipeline. This pipe will be used for item rendering
     */
    Item(bool is_visible, MeshID mesh_id, const VertexPack& vertex_pack, const Color& item_color, bool is_mutable,
         ItemID item_id, const RenderParameters& render_parameters, PipeID pipe_id, TextureID texture_id);

    /**
     * @brief Constructor for fixed geometry Item
     * @param mesh_id - the ID of the scene item mesh
     * @param item_color - the item's material color
     * @param item_id - the ID of an item. It will be taken into account while selecting and hovering items.
     * If equal to 0 then item could not be selected or hovered.
     * (WARNING: only first 24 bits metter!)
     * @param render_parameters - the data structure for the OpenGL pipeline. These parameters will be used for item
     * rendering.
     * @param pipe_id - the ID of the predefined pipeline. This pipe will be used for item rendering
     */
    Item(MeshID mesh_id, const Color& item_color, ItemID item_id = 0, const RenderParameters& render_parameters = {},
         PipeID pipe_id       = gl_scene::defaults::pipes::id::k3D,
         TextureID texture_id = gl_scene::defaults::textures::id::kCommon);

    /**
     * @brief Constructor for mutable Item
     * @param vertex_pack - the container with an item's vertices (is used for rendering in case the item has mutable
     * geometry)
     * @param item_color - the item's material color
     * @param item_id - the ID of an item. It will be taken into account while selecting and hovering items.
     * If equal to 0 then item could not be selected or hovered.
     * (WARNING: only first 24 bits metter!)
     * @param render_parameters - the data structure for the OpenGL pipeline. These parameters will be used for item
     * rendering.
     * @param pipe_id - the ID of the predefined pipeline. This pipe will be used for item rendering
     */
    Item(const VertexPack& vertex_pack, const Color& item_color, ItemID item_id = 0,
         const RenderParameters& render_parameters = {}, PipeID pipe_id = gl_scene::defaults::pipes::id::k3D,
         TextureID texture_id = gl_scene::defaults::textures::id::kCommon);

    /**
     * @brief Constructor for mutable Item
     * @param vertex_pack - the container with an item's vertices (is used for rendering in case the item has mutable
     * geometry)
     * @param item_id - the ID of an item. It will be taken into account while selecting and hovering items.
     * If equal to 0 then item could not be selected or hovered.
     * (WARNING: only first 24 bits metter!)
     * @param render_parameters - the data structure for the OpenGL pipeline. These parameters will be used for item
     * rendering.
     * @param pipe_id - the ID of the predefined pipeline. This pipe will be used for item rendering
     */
    Item(const VertexPack& vertex_pack, ItemID item_id = 0, const RenderParameters& render_parameters = {},
         PipeID pipe_id       = gl_scene::defaults::pipes::id::k3D,
         TextureID texture_id = gl_scene::defaults::textures::id::kCommon);

    /**
     * @brief Constructor for mutable Item
     * @param mesh_id - the ID of the scene item mesh
     * @param item_id - the ID of an item. It will be taken into account while selecting and hovering items.
     * If equal to 0 then item could not be selected or hovered.
     * (WARNING: only first 24 bits metter!)
     * @param render_parameters - the data structure for the OpenGL pipeline. These parameters will be used for item
     * rendering.
     * @param pipe_id - the ID of the predefined pipeline. This pipe will be used for item rendering
     */
    Item(MeshID mesh_id, ItemID item_id = 0, const RenderParameters& render_parameters = {},
         PipeID pipe_id       = gl_scene::defaults::pipes::id::k3D,
         TextureID texture_id = gl_scene::defaults::textures::id::kCommon);

    bool isVisible;
    bool isMutableGeometry;
    ItemID id;
    Color color;
    MeshID meshId;
    Mat4 transformation;
    RenderParameters renderParameters;
    VertexPack vertexPack;
    PipeID pipeId;
    TextureID textureId;
    Texture::Ptr texture;
};

}  // namespace gl_scene
