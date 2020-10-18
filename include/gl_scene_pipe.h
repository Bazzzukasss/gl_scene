#pragma once

#include "gl_scene_types.h"
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <memory>

namespace gl_scene
{
/**
 * The Pipe Class
 * @brief The class is a wrapper on the low level OpenGL structures (such as buffers and shader programs).
 * It contains all necessary data for initialization of the OpenGL pipeline.
 * It is used to adjust the OpenGL pipeline before scene rendering.
 */
class Pipe : public QOpenGLFunctions
{
 public:
    struct Attribute
    {
        GLint size;
        GLsizei stride;
        GLint shift;
    };
    using Attributes = std::vector<Attribute>;

    /**
     * @brief Constructor for the Pipe
     * @param shader - the structure with the source code for the shader processors of the video adapter pipeline
     * @param data - the pointer to a data
     * @param count - the data size in bytes
     * @param attributes - the container with the attributes' data
     */
    Pipe(const Shader& shader, const void* data, int count, const Pipe::Attributes& attributes);

    /**
     * @brief Creates all necessary data of the Pipe
     */
    void create();

    /**
     * @brief Binds the pipe to the OpenGL pipeline (and makes it the current one). The Pipe's program, VAO and VBO will
     * be bound to the OpenGL pipeline.
     */
    void bind();

    /**
     * @brief Unbinds the pipe from the OpenGL pipeline. The Pipe's program, VAO and VBO will be unbound from the OpenGL
     * pipeline.
     */
    void release();

    /**
     * @brief Reallocates the vertex buffer with new data
     * @param data - the pointer to a new data
     * @param count - the data size in bytes
     */
    void allocate(const void* data, int count);

    /**
     * @brief Loads shader program from the source code
     * @param shader - the structure with the source code for the shader processors of the video adapter pipeline
     */
    void addShaderFromSourceCode(const Shader& shader);

    /**
     * @brief Sets the attributes for the OpenGL pipeline rendering
     * @param attributes - the container with the attributes' data
     */
    void addAttributes(const Attributes& attributes);

    /** setters */
    void setView(const Vec3& position, const Mat4& projection, const Mat4& view);
    void setTransform(const Mat4& model);

 protected:
    QOpenGLShaderProgram program;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
};

/**
 * Class PipeExt
 * @brief Extending of base pipe class.
 * Has possibility to set some specific date into openLG pipeline.
 */
class PipeExt : public Pipe
{
 public:
    using Ptr  = std::shared_ptr<PipeExt>;
    using Pack = std::map<PipeID, PipeExt::Ptr>;

    PipeExt(const Shader& shader, const void* pipe_data, int count, const Pipe::Attributes& attributes);

    /** setters */
    void setLight(const Light& light);
    void setColor(const Color& color);
    void setAlfa(float alfa);
};

}  // namespace gl_scene
