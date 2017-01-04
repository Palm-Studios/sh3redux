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

/** Basic vertex type */
typedef struct
{
    GLfloat x, y, z;      /**< Position */
} vertex3f_t;

/** Textured Vertex with normals */
typedef struct
{
    Glfloat x, y, z;
    Glfloat u, v;         /**< Texture UV co-ords */
    GLfloat nx, ny, nz;   /**< Vertex normals (do we need 3??) */
} vertex3f_tex_t;

/** Basic vertex type */
typedef struct
{
    GLfloat x, y, z, w;      /**< Position */
} vertex4f_t;

/** Textured Vertex with normals */
typedef struct
{
    float x, y, z, w;
    float u, v;         /**< Texture UV co-ords */
    float nx, ny, nz;   /**< Vertex normals (do we need 3??) */
} vertex4f_tex_t;

#endif // VERTEX_HPP_INCLUDED
