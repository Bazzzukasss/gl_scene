#pragma once

#include "gl_scene_types.h"
#include <QWidget>

/**
 * The GLSceneGlass Widget
 * @brief The widget for visualization of the scene's text items, selection and zooming rectangle
 */
class GLSceneGlass : public QWidget
{
    Q_OBJECT
 public:
    explicit GLSceneGlass(QWidget* parent = nullptr);

    /** setters */
    void setFrameRect(const gl_scene::Rect& rect);
    void setItems(const gl_scene::TextItem::Pack& item_pack);
    void setFrameVisibility(bool is_visible);
    void setTransformation(const gl_scene::Mat4& transformation);

 protected:
    void paintEvent(QPaintEvent* event) override;

 private:
    bool mIsSelectionFrameVisible;
    gl_scene::Rect mSelectionRect;
    gl_scene::TextItem::Pack mItemPack;
    gl_scene::Mat4 mTransformation;
};
