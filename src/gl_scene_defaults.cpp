#include "gl_scene_defaults.h"
#include "gl_scene_generator.h"

namespace gl_scene
{

namespace defaults
{
namespace common
{
extern const int kSelectionMask{5};
}

namespace cameras
{

const Vec3 kPosition{0.0f, -50.0f, 100.0f};
const Vec3 kWorldUp{0.0f, 0.0f, 1.0f};
const float kYaw{90.0f};
const float kPitch{-45.0f};
const float kSpeed{1.0f};
const float kSensitivity{1.0f};
const float kZoom{1.0f};
const Vec3 kScale{1.0f, 1.0f, 1.0f};
const ProjectionOrtho kOrthoProjection{-150.0f, 150.0f, -50.0f, 250.0f, 0.0f, 250.0f, 1.0};
const ProjectionPerspective kPerspectiveProjection{45.0f, 0.1f, 400.0f, 1.0f};
const Projection::Type kProjectionType{Projection::Type::kPerspective};
const Camera::RangeLimits kRangeLimits{{true, -150.0f, 150.0f}, {true, -50.0f, 250.0f}, {true, 0.2f, 250.0f},
                                       {true, 1.0f, 10.0f},     {true, -89.0f, 0.0f},   {true, 0.0f, 360.0f}};

const Camera kDefault{kPosition,        kWorldUp,    kYaw,   kPitch,          kSpeed,
                      kSensitivity,     kZoom,       kScale, kProjectionType, kPerspectiveProjection,
                      kOrthoProjection, kRangeLimits};
}  // namespace cameras

namespace colors
{
extern const Color kSceneBackground{Qt::black};
extern const Color kSelectedItem{Qt::white};
extern const Color kHoveredItem{Qt::magenta};
extern const Color kSelectionRect{Qt::yellow};
extern const Color kDefault{Qt::white};
}  // namespace colors

namespace lights
{

const Vec3 kDirection{-1.0f, -1.0f, 0.0f};
const Color kAmbient{0x999999};
const Color kDiffuse{0x999999};
const Color kSpecular{Qt::white};

const Light kDefault{kDirection, kAmbient, kDiffuse, kSpecular};
}  // namespace lights

namespace meshes
{

namespace id
{

const MeshID kCube{0};
const MeshID kConus{1};
const MeshID kCylinder{2};
const MeshID kSphere{3};
const MeshID kOktaeder{4};
const MeshID kGrid{5};
}  // namespace id

const Mesh kGrid{GENERATE(FigureGrid{-250, 250, -250, 250, 1, 1}), false};
const Mesh kCube{GENERATE(FigureCube{1.0f}), 0.0f};
const Mesh kConus{GENERATE(FigureConus{0.5f, 1.0f, 32}), 0.0f};
const Mesh kCylinder{GENERATE(FigureCylinder{0.5f, 1.0f, 32, 0.0f}), 0.0f};
const Mesh kSphere{GENERATE(FigureSphere{0.5f, 64, 32}), 0.0f};
const Mesh kOktaeder{GENERATE(FigureSphere{0.3f, 4, 2}), 0.0f};

const Mesh::Map kDefault{{id::kCube, kCube},     {id::kConus, kConus},       {id::kCylinder, kCylinder},
                         {id::kSphere, kSphere}, {id::kOktaeder, kOktaeder}, {id::kGrid, kGrid}};
}  // namespace meshes

namespace pipes
{

namespace id
{

const PipeID kTexturedFlat{0};
const PipeID k2D{1};
const PipeID k3D{2};
const PipeID k3DTextured{3};
const PipeID k2DTextured{4};
const PipeID kSelection{5};
const PipeID k2DLast{6};
const PipeID k3DLast{7};
const PipeID kTexturedFlatLast{8};
const PipeID k3DTexturedLast{9};
const PipeID k2DTexturedLast{10};
}  // namespace id

}  // namespace pipes

namespace shaders
{

// TO DO
// Implement the openGL version identification in runtime
// clang-format off

#define OPENGL_VERSION_3_0  30
#define OPENGL_VERSION_3_1  31
#define OPENGL_VERSION_3_2  32
#define OPENGL_VERSION_3_3  33
#define OPENGL_VERSION_4_0  40
#define OPENGL_VERSION_4_1  41
#define OPENGL_VERSION_4_2  42
#define OPENGL_VERSION_4_3  43
#define OPENGL_VERSION_4_4  44
#define OPENGL_VERSION_4_5  45
#define OPENGL_VERSION_4_6  46

#define OPENGL_VERSION OPENGL_VERSION_3_3

#if(OPENGL_VERSION == OPENGL_VERSION_3_0)
#define SHADER_VERSION "#version 130\n#extension GL_ARB_explicit_attrib_location : enable\n"
#endif

#if(OPENGL_VERSION == OPENGL_VERSION_3_1)
#define SHADER_VERSION "#version 140\n#extension GL_ARB_explicit_attrib_location : enable\n"
#endif

#if(OPENGL_VERSION == OPENGL_VERSION_3_2)
#define SHADER_VERSION "#version 150\n#extension GL_ARB_explicit_attrib_location : enable\n"
#endif

#if(OPENGL_VERSION == OPENGL_VERSION_3_3)
#define SHADER_VERSION "#version 330 core\n"
#endif

#if(OPENGL_VERSION == OPENGL_VERSION_4_0)
#define SHADER_VERSION "#version 400 core\n"
#endif

#if(OPENGL_VERSION == OPENGL_VERSION_4_1)
#define SHADER_VERSION "#version 410 core\n"
#endif

#if(OPENGL_VERSION == OPENGL_VERSION_4_2)
#define SHADER_VERSION "#version 420 core\n"
#endif

#if(OPENGL_VERSION == OPENGL_VERSION_4_3)
#define SHADER_VERSION "#version 430 core\n"
#endif

#if(OPENGL_VERSION == OPENGL_VERSION_4_4)
#define SHADER_VERSION "#version 440 core\n"
#endif

#if(OPENGL_VERSION == OPENGL_VERSION_4_5)
#define SHADER_VERSION "#version 450 core\n"
#endif

#if(OPENGL_VERSION == OPENGL_VERSION_4_6)
#define SHADER_VERSION "#version 460 core\n"
#endif

const Shader k3DPipe{
    SHADER_VERSION
    "layout (location = 0) in vec3 aPos;\n\
    layout (location = 1) in vec3 aNormal;\n\
    out vec3 Normal;\n\
    out vec3 FragPos;\n\
    uniform mat3 normal;\n\
    uniform mat4 model;\n\
    uniform mat4 view;\n\
    uniform mat4 projection;\n\
    void main()\n\
    {\n\
        gl_Position = projection * view * model * vec4(aPos, 1.0);\n\
        FragPos = vec3(model * vec4(aPos, 1.0));\n\
        Normal = normal * aNormal;\n\
    }",

    SHADER_VERSION
    "struct Light {\n\
        vec3 direction;\n\
        vec3 ambient;\n\
        vec3 diffuse;\n\
        vec3 specular;\n\
    };\
    in vec3 Normal;\n\
    in vec3 FragPos;\n\
    out vec4 FragColor;\n\
    uniform Light light;\n\
    uniform vec3 color;\n\
    uniform vec3 viewPos;\n\
    uniform vec3 lightPos;\n\
    uniform vec3 lightColor;\n\
    uniform vec3 objectColor;\n\
    uniform float alfa = 1.0;\n\
    float specularStrength = 0.3;\n\
    float shinines = 128.0;\n\
    void main()\n\
    {\n\
        // ambient\n\
        vec3 ambient = light.ambient * color;\n\
        // diffuse\n\
        vec3 norm = normalize(Normal);\n\
        vec3 lightDir = normalize(-light.direction);\n\
        float diff = max(dot(norm, lightDir), 0.0);\n\
        vec3 diffuse = light.diffuse * (diff * color);\n\
        // specular\n\
        vec3 viewDir = normalize(viewPos - FragPos);\n\
        vec3 reflectDir = reflect(-lightDir, norm);\n\
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shinines);\n\
        vec3 specular = light.specular * spec * specularStrength;\n\
        vec3 result = ambient + diffuse + specular;\n\
        FragColor = vec4(result, alfa);\n\
    }"
};

const Shader k2DPipe{
    SHADER_VERSION
    "layout (location = 0) in vec3 aPos;\n\
    layout (location = 1) in vec3 aNormal;\n\
    out vec3 Normal;\n\
    out vec3 FragPos;\n\
    uniform mat3 normal;\n\
    uniform mat4 model;\n\
    uniform mat4 view;\n\
    uniform mat4 projection;\n\
    void main()\n\
    {\n\
        gl_Position = projection * view * model * vec4(aPos, 1.0);\n\
        FragPos = vec3(model * vec4(aPos, 1.0));\n\
        Normal = normal * aNormal;\n\
    }",

    SHADER_VERSION
    "in vec3 FragPos;\n\
    out vec4 FragColor;\n\
    uniform vec3 color;\n\
    uniform float alfa = 1.0;\n\
    void main()\n\
    {\n\
        FragColor = vec4(color, alfa);\n\
    }"
};

const Shader kSelectionPipe{
    SHADER_VERSION
    "layout (location = 0) in vec3 aPos;\n\
    layout (location = 1) in vec3 aNormal;\n\
    out vec3 Normal;\n\
    out vec3 FragPos;\n\
    uniform mat3 normal;\n\
    uniform mat4 model;\n\
    uniform mat4 view;\n\
    uniform mat4 projection;\n\
    void main()\n\
    {\n\
        gl_Position = projection * view * model * vec4(aPos, 1.0);\n\
        FragPos = vec3(model * vec4(aPos, 1.0));\n\
        Normal = normal * aNormal;\n\
    }",

    SHADER_VERSION
    "in vec3 FragPos;\n\
    out vec4 FragColor;\n\
    uniform vec3 color;\n\
    void main()\n\
    {\n\
        FragColor = vec4(color, 1.0);\n\
    }"
};

const Shader k3DPipeTextured{
    SHADER_VERSION
    "layout (location = 0) in vec3 aPos;\n\
    layout (location = 1) in vec3 aNormal;\n\
    layout (location = 2) in vec2 aTexCoord;\n\
    out vec3 Normal;\n\
    out vec3 FragPos;\n\
    out vec2 TexCoord;\n\
    uniform mat3 normal;\n\
    uniform mat4 model;\n\
    uniform mat4 view;\n\
    uniform mat4 projection;\n\
    void main()\n\
    {\n\
        gl_Position = projection * view * model * vec4(aPos, 1.0);\n\
        FragPos = vec3(model * vec4(aPos, 1.0));\n\
        Normal = normal * aNormal;\n\
        TexCoord = aTexCoord;\n\
    }",

    SHADER_VERSION
    "struct Light {\n\
        vec3 direction;\n\
        vec3 ambient;\n\
        vec3 diffuse;\n\
        vec3 specular;\n\
    };\
    in vec3 Normal;\n\
    in vec3 FragPos;\n\
    in vec2 TexCoord;\n\
    out vec4 FragColor;\n\
    uniform Light light;\n\
    uniform vec3 color;\n\
    uniform vec3 viewPos;\n\
    uniform vec3 lightPos;\n\
    uniform vec3 lightColor;\n\
    uniform vec3 objectColor;\n\
    uniform float alfa = 1.0;\n\
    uniform sampler2D textureData;\n\
    float specularStrength = 0.3;\n\
    float shinines = 128.0;\n\
    void main()\n\
    {\n\
        // ambient\n\
        vec3 ambient = light.ambient * color;\n\
        // diffuse\n\
        vec3 norm = normalize(Normal);\n\
        vec3 lightDir = normalize(-light.direction);\n\
        float diff = max(dot(norm, lightDir), 0.0);\n\
        vec3 diffuse = light.diffuse * (diff * color);\n\
        // specular\n\
        vec3 viewDir = normalize(viewPos - FragPos);\n\
        vec3 reflectDir = reflect(-lightDir, norm);\n\
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shinines);\n\
        vec3 specular = light.specular * spec * specularStrength;\n\
        vec3 result = ambient + diffuse + specular;\n\
        FragColor = texture(textureData, TexCoord) * vec4(result, alfa);\n\
    }"
};

const Shader k2DPipeTextured{
    SHADER_VERSION
    "layout (location = 0) in vec3 aPos;\n\
    layout (location = 1) in vec3 aNormal;\n\
    layout (location = 2) in vec2 aTexCoord;\n\
    out vec3 Normal;\n\
    out vec3 FragPos;\n\
    out vec2 TexCoord;\n\
    uniform mat3 normal;\n\
    uniform mat4 model;\n\
    uniform mat4 view;\n\
    uniform mat4 projection;\n\
    void main()\n\
    {\n\
        gl_Position = projection * view * model * vec4(aPos, 1.0);\n\
        FragPos = vec3(model * vec4(aPos, 1.0));\n\
        Normal = normal * aNormal;\n\
        TexCoord = aTexCoord;\n\
    }",

    SHADER_VERSION
    "in vec3 FragPos;\n\
    in vec2 TexCoord;\n\
    out vec4 FragColor;\n\
    uniform vec3 color;\n\
    uniform float alfa = 1.0;\n\
    uniform sampler2D textureData;\n\
    void main()\n\
    {\n\
        FragColor = texture(textureData, TexCoord) * vec4(color, alfa);\n\
    }"
};

const Shader kPipeTexturedFlat{
    SHADER_VERSION
    "layout (location = 0) in vec3 aPos;\n\
    layout (location = 1) in vec3 aNormal;\n\
    layout (location = 2) in vec2 aTexCoord;\n\
    out vec2 TexCoord;\n\
    uniform mat3 normal;\n\
    uniform mat4 model;\n\
    uniform mat4 view;\n\
    uniform mat4 projection;\n\
    void main()\n\
    {\n\
        gl_Position = vec4(aPos, 1.0);\n\
        TexCoord = aTexCoord;\n\
    }",

    SHADER_VERSION
    "in vec2 TexCoord;\n\
    out vec4 FragColor;\n\
    uniform vec3 color;\n\
    uniform float alfa = 1.0;\n\
    uniform sampler2D textureData;\n\
    void main()\n\
    {\n\
        FragColor = texture(textureData, TexCoord) * vec4(color, alfa);\n\
    }"
};
// clang-format on

const Shader::Map kDefault{{pipes::id::k2D, k2DPipe},
                           {pipes::id::k3D, k3DPipe},
                           {pipes::id::k2DTextured, k2DPipeTextured},
                           {pipes::id::k3DTextured, k3DPipeTextured},
                           {pipes::id::kTexturedFlat, kPipeTexturedFlat},
                           {pipes::id::kSelection, kSelectionPipe},
                           {pipes::id::k2DLast, k2DPipe},
                           {pipes::id::k3DLast, k3DPipe},
                           {pipes::id::kTexturedFlatLast, kPipeTexturedFlat},
                           {pipes::id::k2DTexturedLast, k2DPipeTextured},
                           {pipes::id::k3DTexturedLast, k3DPipeTextured}};

}  // namespace shaders

namespace textures
{

namespace id
{

const TextureID kCommon{0};
}  // namespace id

const TexturesMap kDefault{};
}  // namespace textures

}  // namespace defaults

}  // namespace gl_scene
