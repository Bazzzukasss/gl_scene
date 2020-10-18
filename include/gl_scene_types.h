#pragma once

#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <memory>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <QGL>

namespace gl_scene
{

typedef uint32_t MeshID;
typedef uint32_t PipeID;
typedef uint32_t ItemID;
typedef uint32_t TextureID;

using Mat4       = QMatrix4x4;
using Mat3       = QMatrix3x3;
using Vec3       = QVector3D;
using Vec2       = QVector2D;
using Vertex     = std::array<float, 8>;
using VertexPack = std::vector<Vertex>;
using Point2     = std::pair<float, float>;
using Point2Pack = std::vector<Point2>;
using Point3     = std::array<float, 3>;
using Point3Pack = std::vector<Point3>;
using Color      = QColor;
using Rect       = QRect;

struct Light
{
    Vec3 direction;
    Color ambient;
    Color diffuse;
    Color specular;
};

struct GeometryData
{
    GLint first;
    GLsizei count;
};

struct FigureLine
{
    float width;
    float length;
};

struct FigureStripe
{
    Point3Pack upperShape;
    Point3Pack lowerShape;
};

struct FigureTube
{
    Point3Pack shape;
    float height;
};

struct FigureCube
{
    float size;
};

struct FigureWireBox
{
    float size;
    bool isCrossed;
};

struct FigureSphere
{
    float radius;
    uint hSides;
    uint vSides;
};

struct FigureCylinder
{
    float radius;
    float height;
    uint sides;
    float angle;
};

struct FigureConus
{
    float radius;
    float height;
    uint sides;
};

struct FigureGrid
{
    float minX;
    float maxX;
    float minY;
    float maxY;
    float cellSizeX;
    float cellSizeY;
};

struct FigureExtrusion
{
    Point3Pack path;
    Point3Pack shape;
};

struct FigurePath
{
    Point3Pack path;
    float width;
};

struct FigureFOV
{
    QPointF mountingPoint;
    double mountingAngle;
    double primaryRange;
    double primaryFov;
    double secondaryRange;
    double secondaryFov;
};

struct Shader
{
    using Map = std::map<PipeID, Shader>;
    std::string mVertex;
    std::string mFragment;
};

struct RenderAttributes
{
    float lineWidth;
    std::vector<GLenum> enableAttributes;
    std::vector<GLenum> disableAttributes;
};

struct RenderParameters
{
    RenderParameters(GLenum render_mode = GL_TRIANGLES, float render_alfa = 1.0f,
                     const RenderAttributes& render_attributes = {1.0f, {}, {}}) :
        mode(render_mode),
        alfa(render_alfa),
        attributes(render_attributes)
    {}
    GLenum mode;
    float alfa;
    RenderAttributes attributes;
};

struct TextItem
{
    using Pack = std::vector<TextItem>;
    inline bool isEmpty() const { return text.isEmpty(); }
    QString text;
    Vec3 position;
    Color color;
    QFont font;
    int shiftX;
    int shiftY;
    bool isHoldInScreen;
};

struct Texture
{
    using Ptr = std::shared_ptr<QOpenGLTexture>;
    QString filename;
    Ptr data;
};

using TexturesMap = std::map<TextureID, Texture>;

}  // namespace gl_scene
