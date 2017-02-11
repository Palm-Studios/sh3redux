/** @file
 *
 *  Implementation of quad.hpp
 *
 *  @copyright 2017 Palm Studios
 *
 *  @date 11-2-2017
 *
 *  @author Jesse Buhagiar
 */
#include "SH3/graphics/quad.hpp"

using namespace sh3_graphics;

quad::quad(const std::array<vertex3f, 6>& verts)
{
    // First, we generate our VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Now generate our buffer
    glGenBuffers(1, &vbuff);
    glBindBuffer(GL_ARRAY_BUFFER, vbuff); // Bind this VBO as our vertex storage (GL_ARRAY_BUFFER)

    // Copy the vertices into our buffer
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vertex3f), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // 3-floats per vertex, in VAO slot 0 (position/vertex)
    glBindVertexArray(0);
}

void quad::Draw()
{
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 6); // We already know we have 6-vertexes for a quad.
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}