#pragma once

#include "gl_scene_camera.h"

namespace gl_scene
{

class Manipulator
{
    const int kDefaultZoomSensitivity{120};

 public:
    using Ptr = std::shared_ptr<Manipulator>;

    Manipulator(const Camera& initial_camera);
    virtual ~Manipulator() = default;

    /** setters */
    inline void setZoomSensitivity(int value) { mZoomSensitivity = value; }
    inline void setRectZoomMode(bool mode) { mIsRectZoomMode = mode; }
    inline void setRectSelectionMode(bool mode) { mIsRectSelectionMode = mode; }

    /** getters */
    inline int getZoomSensitivity() const { return mZoomSensitivity; }
    inline bool isRectZoomMode() const { return mIsRectZoomMode; }
    inline bool isRectSelectionMode() const { return mIsRectSelectionMode; }
    inline bool isDragMode() const { return mIsDragMode; }

    /** methods for camera manipulation */
    inline void reset(Camera& camera) { camera.resetTo(mInitialCamera); }
    virtual void focus(const Vec3&, Camera&) {}
    virtual void shift(int dx, int dy, Camera& camera)   = 0;
    virtual void move(int dx, int dy, Camera& camera)    = 0;
    virtual void rotate(int dx, int dy, Camera& camera)  = 0;
    virtual void zoom(int delta, Camera& camera)         = 0;
    virtual void zoom(const QRect& rect, Camera& camera) = 0;

    virtual void mousePressEvent(QMouseEvent* event, Camera& camera);
    virtual void mouseReleaseEvent(QMouseEvent* event, Camera& camera);
    virtual void mouseMoveEvent(QMouseEvent*, Camera&);
    virtual void wheelEvent(QWheelEvent*, Camera&);
    virtual void keyPressEvent(QKeyEvent*, Camera&) {}

 protected:
    int mCurX;
    int mCurY;
    int mPressedX;
    int mPressedY;
    Camera mInitialCamera;
    bool mIsRectSelectionMode{false};
    bool mIsRectZoomMode{false};
    bool mIsDragMode{false};
    int mZoomSensitivity{kDefaultZoomSensitivity};
};

class StandartManipulator : public Manipulator
{
 public:
    StandartManipulator(const Camera& initial_camera);

    void focus(const Vec3& point, Camera& camera) override;
    void move(int dx, int dy, Camera& camera) override;
    void shift(int dx, int dy, Camera& camera) override;
    void rotate(int dx, int dy, Camera& camera) override;
    void zoom(int delta, Camera& camera) override;
    void zoom(const QRect& rect, Camera& camera) override;
};

class FixedManipulator : public Manipulator
{
 public:
    FixedManipulator(const Camera& initial_camera) : Manipulator(initial_camera) {}

    void focus(const Vec3&, Camera&) override {}
    void move(int, int, Camera&) override {}
    void shift(int, int, Camera&) override {}
    void rotate(int, int, Camera&) override {}
    void zoom(int, Camera&) override {}
    void zoom(const QRect&, Camera&) override {}
};

}  // namespace gl_scene
