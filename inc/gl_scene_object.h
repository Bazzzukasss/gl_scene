#pragma once

#include "gl_scene_item.h"

#define DECLARE_SCENE_OBJECT_CLASS(name) \
    struct name##Data; \
    using name = SceneObjectExt<name##Data>; \
    struct name##Data

namespace gl_scene
{

/**
 * The SceneObject Class
 * @brief The class is the base class for any scene's object.
 * It should be used for implementation of the scene's objects with sophisticated behaviour that are more compex then
 * just the scene's item.
 */
class SceneObject
{
 public:
    using Ptr     = std::shared_ptr<SceneObject>;
    using PtrPack = std::vector<SceneObject::Ptr>;
    /**
     * @brief Constructor for SceneObject
     * @param items - the container with the items that the object will operate on
     */
    SceneObject();
    virtual ~SceneObject() = default;

    /**
     * @brief update the object's state
     */
    void update();

    /**
     * @brief adds the child object
     * @param obj_ptr - the shared pointer on added object
     */
    inline void addObject(SceneObject::Ptr obj_ptr) { mObjectPtrPack.emplace_back(obj_ptr); }

    /** setters */
    virtual void setVisibility(bool is_visible);

    /** getters */
    inline bool isVisible() const { return mIsVisible; }
    inline const Item::PtrPack& getItems() const { return mItemPtrPack; }
    inline const PtrPack getObjects() const { return mObjectPtrPack; }
    Item::PtrPack getAllItems() const;

 protected:
    virtual void selfUpdate() = 0;
    void childrenUpdate();

    bool mIsVisible;
    bool mIsChanged;
    Item::PtrPack mItemPtrPack;
    PtrPack mObjectPtrPack;
};

}  // namespace gl_scene
template <typename T>
class SceneObjectExt : public gl_scene::SceneObject
{
 public:
    using Data    = T;
    using Ptr     = std::shared_ptr<SceneObjectExt<T>>;
    using PtrPack = std::vector<Ptr>;
    SceneObjectExt(gl_scene::Item::Pack items, const T& data = {})
    {
        for (auto item : items)
        {
            mItemPtrPack.emplace_back(std::make_shared<gl_scene::Item>(item));
        }
        setData(data);
    }

    void selfUpdate() override;
    void setData(const T& data)
    {
        mData      = data;
        mIsChanged = true;
    }

 private:
    T mData;
};
