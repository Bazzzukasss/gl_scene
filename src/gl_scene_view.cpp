#include "gl_scene_view.h"
#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QGLFormat>
#include <QtMath>
#include <chrono>
#include <deque>

//#define SHOW_DEBUG

using namespace gl_scene;

GLSceneView::GLSceneView(QWidget* parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(16);
    setFormat(format);

    mManipulator = std::make_shared<StandartManipulator>(mCamera);
    mGlassWidget = new GLSceneGlass(this);
    mGlassWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    setMouseTracking(true);
    makeCurrent();

    connect(&mCamera, &Camera::signalChanged, [&](const Mat4& transformation) {
        mGlassWidget->setTransformation(transformation);
        emit signalCameraChanged(transformation);
        update();
    });
}

GLSceneView::~GLSceneView()
{
    cleanup();
}

void GLSceneView::focusCamera(const Vec3& point)
{
    if (!std::isnan(point.x()) && !std::isnan(point.y()) && !std::isnan(point.z()))
    {
        mManipulator->focus(point, mCamera);
    }
}

void GLSceneView::zoomCamera(int delta)
{
    mManipulator->zoom(delta, mCamera);
}

void GLSceneView::setRectZoomMode(bool mode)
{
    mManipulator->setRectZoomMode(mode);
}

void GLSceneView::setRectSelectionMode(bool mode)
{
    mManipulator->setRectSelectionMode(mode);
}

void GLSceneView::setManipulator(Manipulator::Ptr manipulator)
{
    mManipulator = manipulator;
    mManipulator->reset(mCamera);
    update();
}

void GLSceneView::initializeGL()
{
    initializeOpenGLFunctions();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    mStandartRenderAttributes = {1.0f, {GL_DEPTH_TEST, GL_CULL_FACE, GL_LINE_SMOOTH}, {GL_BLEND}};
    mPickingRenderAttributes  = {1.0f, {GL_DEPTH_TEST, GL_CULL_FACE}, {GL_LINE_SMOOTH, GL_BLEND}};
    setRenderAttributes(mStandartRenderAttributes);

    const auto& vertices = mScene->getVertices();
    const auto& size     = static_cast<int>(vertices.size() * sizeof(Vertex));
    Pipe::Attributes attributes{{3, 8, 0}, {3, 8, 3}, {2, 8, 6}};
    for (auto& shaderPair : mScene->getShaders())
    {
        PipeID pipeId         = shaderPair.first;
        Shader shader         = shaderPair.second;
        mStaticPipes[pipeId]  = std::make_shared<PipeExt>(shader, vertices.data(), size, attributes);
        mDynamicPipes[pipeId] = std::make_shared<PipeExt>(shader, vertices.data(), size, attributes);
    }

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLSceneView::cleanup);
}

void GLSceneView::pickItems(int x1, int y1, int x2, int y2, int mask, bool is_selection)
{
    makeCurrent();

    QOpenGLFramebufferObjectFormat selectionBufferFormat;
    selectionBufferFormat.setSamples(0);
    selectionBufferFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    QOpenGLFramebufferObject mSelectionBuffer(size(), selectionBufferFormat);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, defaultFramebufferObject());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mSelectionBuffer.handle());

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setRenderAttributes(mPickingRenderAttributes);
    paintItems(false);

    mSelectionBuffer.release();

    const QImage& image = mSelectionBuffer.toImage();

    if (is_selection)
    {
        mSelectedItemIds.clear();
    }

    const auto& minX = std::min(x1, x2);
    const auto& maxX = std::max(x1, x2);
    const auto& minY = std::min(y1, y2);
    const auto& maxY = std::max(y1, y2);
    ItemID hoveredItemId{0};

    for (int x{minX}; x <= maxX; x++)
    {
        for (int y{minY}; y <= maxY; y++)
        {
            if ((mask != 1 && ((((x & mask) && (y & mask)) || !((x & mask) || (y & mask))))) || mask == 1)
            {
                if (x < image.width() && y < image.height())
                {
                    const auto& itemId = image.pixel(x, y) & 0xFFFFFF;
                    if (is_selection)
                    {
                        mSelectedItemIds.insert(itemId);
                    }
                    hoveredItemId = itemId;
                }
            }
        }
    }

    if (mHoveredItemId != hoveredItemId)
    {
        mHoveredItemId = hoveredItemId;
        if (mHoveredItemId != 0)
        {
            emit signalHoverChanged(true, mHoveredItemId);
        }
        else
        {
            emit signalHoverChanged(false, mHoveredItemId);
        }
    }
    // image.save(QString("fb1.bmp"), 0, 0);
    if (is_selection)
    {
        emit signalSelectionChanged(mSelectedItemIds);
    }
}

void GLSceneView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    mCamera.setViewPort(w, h);
    mGlassWidget->setGeometry(0, 0, w, h);
}

void GLSceneView::paintGL()
{
#ifdef SHOW_DEBUG
    using namespace std::chrono;
    auto t1 = system_clock::now().time_since_epoch();
#endif

    glClearColor(static_cast<float>(mBackgroundColor.redF()), static_cast<float>(mBackgroundColor.greenF()),
                 static_cast<float>(mBackgroundColor.blueF()), static_cast<float>(mBackgroundColor.alphaF()));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    paintItems();
    paintTextItems();

#ifdef SHOW_DEBUG
    auto t2 = system_clock::now().time_since_epoch();
    qDebug() << "GLSceneView::paintGL duration:" << duration_cast<microseconds>(t2 - t1).count() << "mks";
#endif
}

void GLSceneView::paintItems(bool is_standart_drawing)
{
    PipeExt::Ptr curPipe;
    Texture::Ptr curTexture;
    auto light(mScene->getLight());
    light.direction = mCamera.getFront();

    for (const auto& itemsPair : mScene->getItems())
    {
        const auto& pipeId = is_standart_drawing ? itemsPair.first : defaults::pipes::id::kSelection;
        const auto& items  = itemsPair.second;

        auto& staticPipe  = mStaticPipes[pipeId];
        auto& dynamicPipe = mDynamicPipes[pipeId];

        for (auto& item : items)
        {
            if (item->isVisible)
            {
                // define pipe
                auto& pipe = item->isMutableGeometry ? dynamicPipe : staticPipe;
                if (curPipe != pipe)
                {
                    if (curPipe)
                    {
                        curPipe->release();
                    }
                    pipe->bind();
                    pipe->setLight(light);
                    pipe->setView(mCamera.getPosition(), mCamera.getProjection(), mCamera.getView());
                    curPipe = pipe;
                }

                // define texture
                auto texture = item->texture ? item->texture : mScene->getTexture(item->textureId);
                if (texture && curTexture != texture)
                {
                    if (curTexture)
                    {
                        curTexture->release();
                    }
                    texture->bind();
                    curTexture = texture;
                }

                paintItem(pipe, item, is_standart_drawing);
            }
        }
    }
}

void GLSceneView::paintTextItems()
{
    if (mIsTextVisible)
    {
        mGlassWidget->setItems(mScene->getTextItems());
    }
}

void GLSceneView::paintItem(PipeExt::Ptr pipe, Item::Ptr item, bool is_standart_drawing)
{
    static Color oldColor;
    static float oldAlfa;
    static PipeExt::Ptr oldPipe;
    int count;
    int first;
    Color color;

    if (!is_standart_drawing && item->id == 0)
    {
        return;
    }

    if (item->isMutableGeometry)
    {
        first = 0;
        count = static_cast<int>(item->vertexPack.size());
        pipe->allocate(item->vertexPack.data(), count * static_cast<int>(sizeof(Vertex)));
    }
    else
    {
        const auto& geometryData = mScene->getGeometryData(item->meshId);
        first                    = geometryData.first;
        count                    = geometryData.count;
    }

    if (is_standart_drawing)
    {
        int factor = 100;

        if (item->id != 0)
        {
            if (mSelectedItemIds.count(item->id))
            {
                factor = 190;
            }

            if (item->id == mHoveredItemId)
            {
                factor = factor != 100 ? 160 : 180;
            }
        }

        color = item->color.lighter(factor);
    }
    else
    {
        color = item->id;
    }

    if ((oldColor != color) || oldPipe != pipe)
    {
        pipe->setColor(color);
        oldColor = color;
    }

    pipe->setTransform(item->transformation);
    setRenderAttributes(item->renderParameters.attributes);

    if (oldAlfa != item->renderParameters.alfa || oldPipe != pipe)
    {
        pipe->setAlfa(item->renderParameters.alfa);
        oldAlfa = item->renderParameters.alfa;
    }

    oldPipe = pipe;

    if (!is_standart_drawing)
    {
        glDisable(GL_BLEND);
    }

    pipe->glDrawArrays(item->renderParameters.mode, first, count);
    setRenderAttributes(is_standart_drawing ? mStandartRenderAttributes : mPickingRenderAttributes);
}

void GLSceneView::cleanup()
{
    makeCurrent();
    doneCurrent();
}

void GLSceneView::setRenderAttributes(const RenderAttributes& attributes)
{
    glLineWidth(attributes.lineWidth);

    for (const auto& attr : attributes.enableAttributes)
    {
        glEnable(attr);
    }

    for (const auto& attr : attributes.disableAttributes)
    {
        glDisable(attr);
    }
}

void GLSceneView::updateCursorShape()
{
    if (mManipulator->isDragMode())
    {
        this->setCursor(Qt::CursorShape::OpenHandCursor);
    }
    else if (mManipulator->isRectSelectionMode() || mManipulator->isRectZoomMode())
    {
        this->setCursor(Qt::CursorShape::CrossCursor);
    }
    else
    {
        this->setCursor(Qt::CursorShape::ArrowCursor);
    }
}

void GLSceneView::mousePressEvent(QMouseEvent* event)
{
    mManipulator->mousePressEvent(event, mCamera);

    mCurX = mPressedX = event->x();
    mCurY = mPressedY = event->y();
    mGlassWidget->setFrameRect({QPoint{mPressedX, mPressedY}, QPoint{mCurX, mCurY}});
    mGlassWidget->setFrameVisibility(mManipulator->isRectSelectionMode() || mManipulator->isRectZoomMode());
}

void GLSceneView::mouseReleaseEvent(QMouseEvent* event)
{
    if (mManipulator->isRectSelectionMode())
    {
        pickItems(mPressedX, mPressedY, mCurX, mCurY);
    }
    else
    {
        if ((mPressedX == mCurX) && (mPressedY == mCurY))
        {
            pickItems(mCurX, mCurY, mCurX, mCurY);
        }
    }

    mGlassWidget->setFrameVisibility(false);

    mManipulator->mouseReleaseEvent(event, mCamera);
    updateCursorShape();
}

void GLSceneView::mouseMoveEvent(QMouseEvent* event)
{
    mCurX = event->x();
    mCurY = event->y();

    mGlassWidget->setFrameRect(QRect{QPoint{mPressedX, mPressedY}, QPoint{mCurX, mCurY}});
    pickItems(mCurX, mCurY, mCurX, mCurY, 1, false);

    mCursorPosition = mCamera.toWorldXYCoordinates(mCurX, mCurY);
    emit signalCursorChanged(mCursorPosition);

    mManipulator->mouseMoveEvent(event, mCamera);
    updateCursorShape();
}

void GLSceneView::wheelEvent(QWheelEvent* event)
{
    mManipulator->wheelEvent(event, mCamera);
}

void GLSceneView::keyPressEvent(QKeyEvent* event)
{
    mManipulator->keyPressEvent(event, mCamera);
    updateCursorShape();
}
