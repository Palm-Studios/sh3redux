/** @file
 *
 *  Implementation of glvertexbuffer.hpp
 *
 *  @copyright 2016-2019 Palm Studios
 *
 *  @date 3-2-2019
 *
 *  @author Jesse Buhagiar (quaker762)
 *
 */
#include "SH3/system/glvertexbuffer.hpp"
#include "SH3/system/log.hpp"

#include <cstring>

using namespace sh3::gl;

CVertexBuffer::CVertexBuffer()
    : vboID(VBO_RESET), name("DEFAULT_VERTEX_BUFFER"), target(BufferTarget::ARRAY_BUFFER), data(0x00)
{
    Create();
}

CVertexBuffer::CVertexBuffer(const std::string& _name)
    : vboID(VBO_RESET), name(_name), target(BufferTarget::ARRAY_BUFFER), data(0x00)
{
    Create();
}

CVertexBuffer::~CVertexBuffer()
{
    Destroy();
}

void CVertexBuffer::Create(void) noexcept
{
    glGenBuffers(1, &vboID);
}

void CVertexBuffer::Destroy(void) noexcept
{
    // Free memory from both contexts
    glDeleteBuffers(1, &vboID);
    data.clear();
    data.shrink_to_fit();
}

void CVertexBuffer::Bind() const noexcept
{
    if(vboID > 0)
        glBindBuffer(target, vboID);
    else
        Log(LogLevel::ERROR, "CVertexBuffer::Bind(): vboID (which is %d) for CVertexBuffer %s is invalid!", vboID, name.c_str());
}

void CVertexBuffer::Unbind() const noexcept
{
    glBindBuffer(target, VBO_RESET);
}

void CVertexBuffer::BufferData(BufferTarget target, GLsizeiptr size, const GLvoid* data, BufferUsage usage)
{
    Bind();
    glBufferData(target, size, data, usage);

    this->data.resize(size);
    std::memcpy(this->data.data(), reinterpret_cast<const GLubyte*>(data), size);

    this->target = target;
}

void CVertexBuffer::BufferData(GLsizeiptr size, const GLvoid* data, BufferUsage usage)
{
    glNamedBufferData(vboID, size, data, usage);

    this->data.resize(size);
    std::memcpy(this->data.data(), reinterpret_cast<const GLubyte*>(data), size);
}

void CVertexBuffer::BufferSubData(BufferTarget target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
    Bind();
    glBufferSubData(target, offset, size, data);

    std::memcpy(this->data.data() + offset, reinterpret_cast<const GLubyte*>(data), size);
    this->target = target;

}

void CVertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
    glNamedBufferSubData(vboID, offset, size, data);

    std::memcpy(this->data.data() + offset, reinterpret_cast<const GLubyte*>(data), size);
}
