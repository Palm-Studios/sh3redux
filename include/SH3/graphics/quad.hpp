/** @file
 *
 *  Class to render a fullscreen 2D quad to the screen.
 *
 *  @copyright 2017 Palm Studios
 *
 *  @date 11-2-2017
 *
 *  @note To render a quad, we need to split the screen into two right-angled triangles, meaning we need 6 vertices for one quad.
 *  @note We're not using Index Buffering here because a quad only has 6 verts... Kind of a waste of time,
 *
 *  @author Jesse Buhagiar
 */
#ifndef QUAD_HPP_INCLUDED
#define QUAD_HPP_INCLUDED

#include "GL/glew.h"
#include "GL/gl.h"
#include "glm/glm.hpp"
#include "SH3/types/vertex.hpp"

#include <array>

namespace sh3_graphics
{
    struct quad final
    {
    public:
        /**
         *  Class constructor. Takes
         */
        quad(const std::array<vertex3f, 6>& verts);
        ~quad(){};

        /**
         *  Draw this quad to the screen
         */
        void Draw();

    private:
        GLuint vao;     /**< VAO where we will store the format of the vertex data and buffer objects */
        GLuint vbuff;   /**< The VBO we fill with our vertex data */
    };
}


#endif // QUAD_HPP_INCLUDED
