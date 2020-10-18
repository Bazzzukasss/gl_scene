#pragma once

#include "gl_scene.h"
#include "gl_scene_pipe.h"
#include "gl_scene_camera.h"
#include "gl_scene_glass.h"
#include "gl_scene_manipulator.h"
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <set>

/**
 * The GLSceneView Class
 * @brief The class is the widget for the scene visualisation
 */
class GLSceneView : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

 public:
    GLSceneView(QWidget* parent = nullptr);
    ~GLSceneView() override;

    /**
     * @brief Changes camera position or / and angles in way the point become centered in view port
     * @param point - the point camera's viewport should be centered on
     */
    void focusCamera(const gl_scene::Vec3& point);

    /**
     * @brief Changes cameras zoom
     * @param delta - the value camera should be zoomed by
     */
    void zoomCamera(int delta);

    /** setters */
    inline void setScene(gl_scene::Scene::Ptr scene) { mScene = scene; }
    void setManipulator(gl_scene::Manipulator::Ptr manipulator);
    inline void setBackgroundColor(const gl_scene::Color& color) { mBackgroundColor = color; }
    inline void setSelectedItemIds(const gl_scene::Item::IdPack& ids) { mSelectedItemIds = ids; }
    inline void setTextVisibile(bool is_visible) { mIsTextVisible = is_visible; }
    void setRectZoomMode(bool mode);
    void setRectSelectionMode(bool mode);

    /** getters */
    inline const gl_scene::Vec3& getCursorPosition() const { return mCursorPosition; }
    inline gl_scene::Camera& camera() { return mCamera; }
    inline gl_scene::Manipulator::Ptr getManipulator() const { return mManipulator; }

 signals:
    void signalSelectionChanged(const gl_scene::Item::IdPack& item_Ids);
    void signalHoverChanged(bool is_hovered, gl_scene::ItemID item_id);
    void signalCameraChanged(const gl_scene::Mat4& mvp_matrix);
    void signalCursorChanged(const gl_scene::Vec3& position);

 protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

 private:
    void pickItems(int x1, int y1, int x2, int y2, int mask = 1, bool is_selection = true);
    void paintItems(bool is_standart_drawing = true);
    void paintTextItems();
    void paintItem(gl_scene::PipeExt::Ptr pipe, gl_scene::Item::Ptr item, bool is_standart_drawing = true);
    void cleanup();
    void setRenderAttributes(const gl_scene::RenderAttributes& attributes);
    void updateCursorShape();

    int mCurX;
    int mCurY;
    int mPressedX;
    int mPressedY;
    bool mIsTextVisible{true};
    GLSceneGlass* mGlassWidget;
    gl_scene::Camera mCamera{gl_scene::defaults::cameras::kDefault};
    gl_scene::Manipulator::Ptr mManipulator;
    gl_scene::Mat4 mMVPTransformation;
    gl_scene::Vec3 mPosition{0.0, 0.0, 0.0};
    gl_scene::Vec3 mCursorPosition;
    gl_scene::RenderAttributes mStandartRenderAttributes;
    gl_scene::RenderAttributes mPickingRenderAttributes;
    gl_scene::Scene::Ptr mScene;
    gl_scene::PipeExt::Pack mStaticPipes;
    gl_scene::PipeExt::Pack mDynamicPipes;
    gl_scene::Item::IdPack mSelectedItemIds;
    gl_scene::ItemID mHoveredItemId;
    gl_scene::Color mBackgroundColor{gl_scene::defaults::colors::kSceneBackground};
};
