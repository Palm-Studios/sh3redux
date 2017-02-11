/** @file
 *  Macros and structures related to an OpenGL 3D vertex.
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 14-12-2016
 *
 *  @author Jesse Buhagiar
 */
#ifndef VERTEX_HPP_INCLUDED
#define VERTEX_HPP_INCLUDED

#include <GL/gl.h>

#pragma pack(push, 1)

/** @defgroup graphics-types Graphics Types
 *  @{
*/

/** UV Texture Co-Ordinate  structure*/
struct texcoord
{
    GLfloat u, v;
};

/** Basic vertex type */
struct vertex3f
{
    GLfloat x, y, z;      /**< Position */
};

/** Model Vertex (for static models) */
struct sh3_model_vertex
{
    vertex3f vertex;    /**< Vertex */
    vertex3f normal;    /**< Vertex Normal */
    texcoord uvcoord;   /**< Texture UV Co-Ordinate for this vertex */
};




#pragma pack(pop)

/**@}*/

#endif // VERTEX_HPP_INCLUDED
