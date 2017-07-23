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

constexpr int GL_VBO_UNBIND  = 0;

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
    glBindBuffer(static_cast<GLenum>(buffType), 0);
}

void buffer_object::BufferData(void* data, std::size_t num, GLenum usage)
{
    Bind();
    glBufferData(static_cast<GLenum>(buffType), num, data, usage);
    size = num;
}

void buffer_object::BufferSubData(void* data, std::size_t size)
{
    Bind();
    glBufferSubData(static_cast<GLenum>(buffType), 0, size, data);
}

std::uint32_t buffer_object::GetNumberElements() const
{
    return size / sizeof(GLfloat); // FIXME: What if we're _not_ using GLfloat!?!?!
}