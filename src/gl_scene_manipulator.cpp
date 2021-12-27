#include "gl_scene_manipulator.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QtMath>

using namespace gl_scene;

Manipulator::Manipulator(const Camera& initial_camera) : mInitialCamera(initial_camera)
{}

void Manipulator::mousePressEvent(QMouseEvent* event, Camera&)
{
    mPressedX       = event->x();
    mPressedY       = event->y();
    mIsRectZoomMode = (event->modifiers() == Qt::CTRL || mIsRectZoomMode) && event->buttons() == Qt::LeftButton;
    mIsRectSelectionMode =
        (event->modifiers() == Qt::SHIFT || mIsRectSelectionMode) && event->buttons() == Qt::LeftButton;
}

void Manipulator::mouseReleaseEvent(QMouseEvent*, Camera& camera)
{
    if (mIsRectZoomMode)
    {
        auto x = std::min(mPressedX, mCurX);
        auto y = std::min(mPressedY, mCurY);
        zoom(QRect{x, y, abs(mCurX - mPressedX), abs(mCurY - mPressedY)}, camera);
    }

    mIsDragMode          = false;
    mIsRectZoomMode      = false;
    mIsRectSelectionMode = false;
}

void Manipulator::mouseMoveEvent(QMouseEvent* event, Camera& camera)
{
    const auto& x  = event->x();
    const auto& y  = event->y();
    const auto& dx = x - mCurX;
    const auto& dy = y - mCurY;
    mCurX          = x;
    mCurY          = y;

    if (!mIsRectSelectionMode && !mIsRectZoomMode)
    {
        if (event->buttons() == Qt::LeftButton)
        {
            move(dx, dy, camera);
            mIsDragMode = true;
        }

        if (event->buttons() == Qt::RightButton)
        {
            rotate(dx, dy, camera);
            mIsDragMode = true;
        }

        if (event->buttons() == Qt::MiddleButton)
        {
            shift(dx, dy, camera);
            mIsDragMode = true;
        }
    }
}

void Manipulator::wheelEvent(QWheelEvent* event, Camera& camera)
{
    zoom(event->angleDelta().y(), camera);
}

StandartManipulator::StandartManipulator(const Camera& initial_camera) : Manipulator(initial_camera)
{}

void StandartManipulator::focus(const Vec3& point, Camera& camera)
{
    camera.focus(point);
}

void StandartManipulator::move(int dx, int dy, Camera& camera)
{
    camera.move(Camera::MovementDirection::kRight, -dx);
    camera.move(Camera::MovementDirection::kXYForward, dy);
}

void StandartManipulator::shift(int dx, int dy, Camera& camera)
{
    camera.move(Camera::MovementDirection::kRight, -dx);
    camera.move(Camera::MovementDirection::kUp, dy);
}

void StandartManipulator::rotate(int dx, int dy, Camera& camera)
{
    camera.rotateAround(-dx, -dy);
}

void StandartManipulator::zoom(int delta, Camera& camera)
{
    if (camera.getProjectionType() == Projection::Type::kPerspective)
    {
        float dy = delta / mZoomSensitivity;
        if ((camera.getPosition().z() <= camera.getRangeLimits().rangeZ.min && dy < 0) ||
            camera.getPosition().z() > camera.getRangeLimits().rangeZ.min)
        {
            camera.move(Camera::MovementDirection::kForward, dy);
        }
    }
    else
    {
        float dy = -delta / (static_cast<float>(mZoomSensitivity) * 10.0f);
        camera.scale({dy, dy, 0.0f});
    }
}

void StandartManipulator::zoom(const QRect& rect, Camera& camera)
{
    auto screenH = camera.getViewPortSize().second;
    auto screenW = camera.getViewPortSize().first;

    auto rectCenter    = camera.toWorldXYCoordinates(rect.center().x(), rect.center().y());
    auto deltaPosition = rectCenter - camera.getLookPoint();

    if (camera.getProjectionType() == Projection::Type::kPerspective)
    {
        auto rectLeft   = camera.toWorldXYCoordinates(rect.left(), rect.center().y());
        auto rectRight  = camera.toWorldXYCoordinates(rect.right(), rect.center().y());
        auto rectTop    = camera.toWorldXYCoordinates(rect.center().x(), rect.top());
        auto rectBottom = camera.toWorldXYCoordinates(rect.center().x(), rect.bottom());
        auto rectVSize  = (rectTop - rectBottom).length();
        auto rectHSize  = (rectLeft - rectRight).length();

        auto screenLeft   = camera.toWorldXYCoordinates(0, screenH / 2);
        auto screenRight  = camera.toWorldXYCoordinates(screenW, screenH / 2);
        auto screenTop    = camera.toWorldXYCoordinates(screenW / 2, 0);
        auto screenBottom = camera.toWorldXYCoordinates(screenW / 2, screenH);
        auto screenHSize  = (screenLeft - screenRight).length();
        auto screenVSize  = (screenTop - screenBottom).length();

        float rectWorldSize;
        float screenWorldSize;
        if (rect.width() > rect.height())
        {
            rectWorldSize   = rectHSize;
            screenWorldSize = screenHSize;
        }
        else
        {
            rectWorldSize   = rectVSize;
            screenWorldSize = screenVSize;
        }

        float distance      = rectWorldSize * camera.getLook().length() / screenWorldSize;
        float deltaDistance = camera.getLook().length() - distance;

        camera.moveFront(deltaDistance);
    }
    else
    {
        float k = rect.width() > rect.height() ? static_cast<float>(rect.width()) / static_cast<float>(screenW)
                                               : static_cast<float>(rect.height()) / static_cast<float>(screenH);
        auto scale = camera.getScale();
        camera.setScale({scale.x() * k, scale.y() * k, scale.z()});
    }

    camera.move(deltaPosition);
}
