/** @file
 *
 *  Implementation of @ref buffer.hpp
 *
 *  @copyright 2017 Palm Studios
 *
 *  @date 8-6-2017
 *
 *  @author Jesse Buhagiar
 */
#include "SH3/system/glbuffer.hpp"
#include "SH3/system/log.hpp"

constexpr GLuint GL_VBO_UNBOUND = 0;

using namespace sh3_gl;

void buffer_object::Create()
{
    glGenBuffers(1, &id);
}

void buffer_object::Release()
{
    glDeleteBuffers(1, &id);
}

void buffer_object::Bind() const
{
    glBindBuffer(static_cast<GLenum>(buffType), id);
}

void buffer_object::Unbind()
{
    glBindBuffer(static_cast<GLenum>(buffType), GL_VBO_UNBOUND);
}

void buffer_object::BufferData(void* data, GLsizei dataSize, GLenum usage)
{
    Bind();
    glBufferData(static_cast<GLenum>(buffType), dataSize, data, usage);
    size = dataSize;
}

void buffer_object::BufferSubData(void* data, GLsizei dataSize)
{
    //ASSERT(dataSize <= size);
    Bind();
    glBufferSubData(static_cast<GLenum>(buffType), 0, dataSize, data);
}

GLsizei buffer_object::GetNumberElements() const
{
    return size / static_cast<GLsizei>(sizeof(GLfloat)); // FIXME: What if we're _not_ using GLfloat!?!?!
}
