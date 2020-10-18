#include "gl_scene_glass.h"
#include "gl_scene_defaults.h"

using namespace gl_scene;

GLSceneGlass::GLSceneGlass(QWidget* parent) : QWidget(parent), mIsSelectionFrameVisible(false)
{
    mTransformation.setToIdentity();
}

void GLSceneGlass::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setFont(font());
    if (mIsSelectionFrameVisible)
    {
        painter.setPen(defaults::colors::kSelectionRect);
        painter.drawRect(mSelectionRect);
    }

    for (const auto& item : mItemPack)
    {
        QFontMetrics fm(item.font);
        const auto& sizeX = fm.width(item.text);
        const auto& sizeY = fm.height();

        painter.setFont(item.font);
        painter.setPen(item.color);
        auto position = mTransformation * QVector4D(item.position, 1.0);

        float size = position[3] * 2;
        float sx   = width() / size;
        float sy   = height() / size;
        int x      = static_cast<int>(position[0] * sx + width() / 2 + item.shiftX);
        int y      = static_cast<int>(height() - position[1] * sy - height() / 2 + item.shiftY);

        if (item.isHoldInScreen)
        {
            if (x < 0)
            {
                x = 0;
            }
            if (x > width() - sizeX)
            {
                x = width() - sizeX;
            }
            if (y < sizeY)
            {
                y = sizeY;
            }
            if (y > height())
            {
                y = height();
            }
        }

        painter.drawText(x, y, item.text);
    }

    painter.end();
}

void GLSceneGlass::setFrameRect(const Rect& rect)
{
    mSelectionRect = rect;
    update();
}

void GLSceneGlass::setItems(const TextItem::Pack& item_pack)
{
    mItemPack = item_pack;
    update();
}

void GLSceneGlass::setFrameVisibility(bool is_visible)
{
    mIsSelectionFrameVisible = is_visible;
    update();
}

void GLSceneGlass::setTransformation(const Mat4& transformation)
{
    mTransformation = transformation;
    update();
}
