/*++

Copyright (c) 2016  Palm Studios

Module Name:
        Macros and structures related to an OpenGL 3D vertex

Abstract:

Author:
        Quaker762

Environment:

Notes:

Revision History:
        14-12-2016: Initial Revision [Quaker762]
--*/

#ifndef VERTEX_HPP_INCLUDED
#define VERTEX_HPP_INCLUDED

// Basic vertex type
typedef struct
{
    float x, y, z;      // Position
} vertex3f_t;

// Textured Vertex with normals
typedef struct
{
    float x, y, z;
    float u, v;         // Texture UV co-ords
    float nx, ny, nz;   // Vertex normals (do we need 3??)
} vertex3f_tex_t;

// Basic vertex type
typedef struct
{
    float x, y, z, w;      // Position
} vertex4f_t;

// Textured Vertex with normals
typedef struct
{
    float x, y, z, w;
    float u, v;         // Texture UV co-ords
    float nx, ny, nz;   // Vertex normals (do we need 3??)
} vertex4f_tex_t;


#endif // VERTEX_HPP_INCLUDED
