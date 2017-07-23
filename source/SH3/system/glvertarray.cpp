/** @file
 *
 *  Imeplementation of @ref glvertarray.hpp
 *
 *  @copyright 2017 Palm Studios
 *
 *  @date 8-6-2017
 *
 *  @author Jesse Buhagiar
 */
#include "SH3/system/glvertarray.hpp"
#include "SH3/system/log.hpp"

constexpr int   GL_VAO_UNBIND = 0;

using namespace sh3_gl;

void finalvao::Create()
{
    glGenVertexArrays(1, &id);
}

void finalvao::Release()
{
    glDeleteVertexArrays(1, &id);
}

void finalvao::Bind()
{
    glBindVertexArray(id);

    if(ibo)
        ibo->Bind();
}

void finalvao::Unbind()
{
    glBindVertexArray(GL_VAO_UNBIND);
}

void finalvao::Draw()
{
    Bind();
    if(ibo)
        glDrawElements(GL_TRIANGLES, ibo->GetNumberElements(), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, vertices.GetNumberElements());
}