#include "gl_scene.h"

using namespace gl_scene;

Scene::Scene(const Mesh::Map& mesh_map, const Light& light, const Shader::Map& shader_map,
             const TexturesMap& textures_map) :
    mMeshMap(mesh_map),
    mLight(light),
    mShaderMap(shader_map),
    mTexturesMap(textures_map)
{
    initialize();
}

void Scene::initialize()
{
    int index{0};
    for (const auto& meshPair : mMeshMap)
    {
        const auto& meshID       = meshPair.first;
        const auto& meshVertices = meshPair.second.getVertices();

        std::copy(meshVertices.begin(), meshVertices.end(), std::back_inserter(mVertices));
        GLsizei count            = static_cast<GLsizei>(meshVertices.size());
        mMeshGeometryMap[meshID] = {index, count};
        index += count;
    }
}

Item::Ptr Scene::addItem(const Item& item)
{
    auto itemPtr = std::make_shared<Item>(item);
    addItem(itemPtr);

    return itemPtr;
}

void Scene::addItem(const Item::Ptr& item_ptr)
{
    mItemPtrMap[item_ptr->pipeId].emplace_back(item_ptr);
}

void Scene::addObject(const SceneObject::Ptr& obj_ptr)
{
    mObjectPtrPack.emplace_back(obj_ptr);

    for (const auto& item : obj_ptr->getAllItems())
    {
        addItem(item);
    }
}

void Scene::update()
{
    for (auto& obj : mObjectPtrPack)
    {
        if (obj->isVisible())
        {
            obj->update();
        }
    }
}

GeometryData Scene::getGeometryData(MeshID mesh_id) const
{
    const auto dataPair = mMeshGeometryMap.find(mesh_id);
    if (dataPair != mMeshGeometryMap.cend())
    {
        return dataPair->second;
    }

    return {};
}

Texture::Ptr Scene::getTexture(TextureID texture_id)
{
    auto texturePtr = mTexturesMap.find(texture_id);
    if (texturePtr != mTexturesMap.end())
    {
        auto& texture = texturePtr->second;
        if (!texture.data)
        {
            auto data = std::make_shared<QOpenGLTexture>(QImage(texture.filename).mirrored());
            data->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            data->setMagnificationFilter(QOpenGLTexture::Linear);

            texture.data = data;
        }

        return texture.data;
    }

    return {};
}
