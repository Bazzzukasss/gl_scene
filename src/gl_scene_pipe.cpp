#include "gl_scene_pipe.h"
#include "gl_scene_utility.h"

using namespace gl_scene;

Pipe::Pipe(const Shader& shader, const void* data, int count, const Attributes& attributes)
{
    initializeOpenGLFunctions();
    create();
    addShaderFromSourceCode(shader);
    bind();
    allocate(data, count);
    addAttributes(attributes);
    release();
}

void Pipe::create()
{
    vao.create();
    vbo.create();
    program.create();
}

void Pipe::bind()
{
    program.bind();
    vao.bind();
    vbo.bind();
}

void Pipe::release()
{
    program.release();
    vao.release();
    vbo.release();
}

void Pipe::allocate(const void* data, int count)
{
    if (data != nullptr && count > 0)
    {
        vbo.allocate(data, count);
    }
}

void Pipe::setView(const Vec3& position, const Mat4& projection, const Mat4& view)
{
    program.setUniformValue("viewPos", position);
    program.setUniformValue("projection", projection);
    program.setUniformValue("view", view);
}

void Pipe::addShaderFromSourceCode(const Shader& shader)
{
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, shader.mVertex.c_str());
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, shader.mFragment.c_str());
    program.link();
}

void Pipe::addAttributes(const Attributes& attributes)
{
    GLuint index{0};
    for (const auto& attribute : attributes)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, attribute.size, GL_FLOAT, GL_FALSE,
                              attribute.stride * static_cast<GLsizei>(sizeof(GLfloat)),
                              reinterpret_cast<void*>(sizeof(GLfloat) * static_cast<uint>(attribute.shift)));
        index++;
    }
}

void Pipe::setTransform(const Mat4& model)
{
    program.setUniformValue("normal", model.normalMatrix());
    program.setUniformValue("model", model);
}

PipeExt::PipeExt(const Shader& shader, const void* pipe_data, int count, const Attributes& attributes) :
    Pipe(shader, pipe_data, count, attributes)
{}

void PipeExt::setLight(const Light& light)
{
    program.setUniformValue("light.direction", light.direction);
    program.setUniformValue("light.ambient", toVec3(light.ambient));
    program.setUniformValue("light.diffuse", toVec3(light.diffuse));
    program.setUniformValue("light.specular", toVec3(light.specular));
}

void PipeExt::setColor(const Color& color)
{
    program.setUniformValue("color", toVec3(color));
}

void PipeExt::setAlfa(float alfa)
{
    program.setUniformValue("alfa", alfa);
}
