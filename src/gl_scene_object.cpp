#include "gl_scene_object.h"
#include "gl_scene_defaults.h"

using namespace gl_scene;

SceneObject::SceneObject() : mIsVisible(true), mIsChanged(false)
{}

void SceneObject::update()
{
    if (mIsChanged)
    {
        selfUpdate();
        mIsChanged = false;
    }
    childrenUpdate();
}

void SceneObject::childrenUpdate()
{
    for (auto& obj : mObjectPtrPack)
    {
        obj->update();
    }
}

void gl_scene::SceneObject::setVisibility(bool is_visible)
{
    if (mIsVisible != is_visible)
    {
        mIsVisible = is_visible;

        for (auto& item : mItemPtrPack)
        {
            item->isVisible = is_visible;
        }

        for (auto& obj : mObjectPtrPack)
        {
            obj->setVisibility(is_visible);
        }
    }
}

Item::PtrPack SceneObject::getAllItems() const
{
    Item::PtrPack items;

    items = mItemPtrPack;
    for (auto& obj : mObjectPtrPack)
    {
        const auto& objItems = obj->getAllItems();
        std::copy(objItems.begin(), objItems.end(), std::back_inserter(items));
    }

    return items;
}
