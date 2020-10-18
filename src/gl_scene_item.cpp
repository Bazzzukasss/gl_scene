#include "gl_scene_item.h"

using namespace gl_scene;

Item::Item(bool is_visible, MeshID mesh_id, const VertexPack& vertex_pack, const Color& item_color, bool is_mutable,
           ItemID item_id, const RenderParameters& render_parameters, PipeID pipe_id, TextureID texture_id) :
    isVisible(is_visible),
    isMutableGeometry(is_mutable),
    id(item_id),
    color(item_color),
    meshId(mesh_id),
    renderParameters(render_parameters),
    vertexPack(vertex_pack),
    pipeId(pipe_id),
    textureId(texture_id)
{
    transformation.setToIdentity();
}

Item::Item(MeshID mesh_id, const Color& item_color, ItemID item_id, const RenderParameters& render_parameters,
           PipeID pipe_id, TextureID texture_id) :
    Item(true, mesh_id, {}, item_color, false, item_id, render_parameters, pipe_id, texture_id)
{}

Item::Item(const VertexPack& vertex_pack, const Color& item_color, ItemID item_id,
           const RenderParameters& render_parameters, PipeID pipe_id, TextureID texture_id) :
    Item(true, {}, vertex_pack, item_color, true, item_id, render_parameters, pipe_id, texture_id)
{}

Item::Item(const VertexPack& vertex_pack, ItemID item_id, const RenderParameters& render_parameters, PipeID pipe_id,
           TextureID texture_id) :
    Item(true, {}, vertex_pack, {}, true, item_id, render_parameters, pipe_id, texture_id)
{}

Item::Item(MeshID mesh_id, ItemID item_id, const RenderParameters& render_parameters, PipeID pipe_id,
           TextureID texture_id) :
    Item(true, mesh_id, {}, {}, false, item_id, render_parameters, pipe_id, texture_id)
{}
