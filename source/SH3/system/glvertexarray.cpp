/** @file
 *
 *  Implementation of glvertexarray.hpp
 *
 *  @copyright 2016-2019 Palm Studios
 *
 *  @date 30-1-2019
 *
 *  @author Jesse Buhagiar (quaker762)
 *
 */
#include "SH3/system/glvertexarray.hpp"

using namespace sh3::gl;

CVertexArray::CVertexArray()
    : vaoID(VAO_UNBIND), name("VAO_DEFAULT")
{
    Create();
}

CVertexArray::CVertexArray(const std::string& _name)
    : vaoID(VAO_UNBIND), name(_name)
{
    Create();
}

CVertexArray::CVertexArray(const CVertexArray& rhs)
    : vaoID(rhs.vaoID), name(rhs.name)
{

}

CVertexArray::~CVertexArray()
{
    Destroy();
}

inline void CVertexArray::Create(void) noexcept
{
    glGenVertexArrays(1, &vaoID);
}

inline void CVertexArray::Destroy(void) noexcept
{
    glDeleteVertexArrays(1, &vaoID);
}

void CVertexArray::BindAttribute(const VertexAttribute& attrib, const CVertexBuffer& vbo, VertexAttribute::AttributeType type)
{
    glBindVertexArray(vaoID);
    glBindBuffer(vbo.GetTarget(), vbo.Get());
    glEnableVertexAttribArray(attrib.idx);
    glVertexAttribPointer(attrib.idx, attrib.size, type, attrib.normalize, attrib.stride, reinterpret_cast<const GLvoid*>(attrib.offset));
}

void CVertexArray::BindAttribute(const GLuint idx, const CVertexBuffer& vbo, GLint size, GLboolean normalize, GLsizei stride, GLintptr offset, VertexAttribute::AttributeType type)
{
    glBindVertexArray(vaoID);
    glBindBuffer(vbo.GetTarget(), vbo.Get());
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, size, type, normalize, stride, reinterpret_cast<const GLvoid*>(offset));

}
