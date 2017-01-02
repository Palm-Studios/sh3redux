/** @file
 *
 *  Data structures and class to load and draw a SILENT HILL 3 Model file
 *
 *  \copyright 2016  Palm Studios and Mike M (<a href="https://twitter.com/perdedork">\@perdedork</a>)
 *
 *  \note
 *
 *  \date 27-12-2016
 *
 *  \author Jesse Buhagiar
 */
#ifndef SH3_MODEL_HPP_INCLUDED
#define SH3_MODEL_HPP_INCLUDED

#include <cstdint>

typedef struct
{
    std::uint32_t unused1;               // Unused DWORD
    std::uint32_t modelID;               // Model Identifier (not sure how this was used yet)
    std::uint32_t numTextures;           // Number of textures for this model (1 model can contain many textures, i.e, Heather and Memory of Alessa)
    std::uint32_t textureDataoffset;     // Offset to the start of the (first??) texture.
    std::uint32_t headerSize;            // Size of this header
    std::uint32_t unused2;               // Unused DWORD
    std::uint32_t unused[8];             // Jesus Christ, KONAMI.....
} sh3_model_header_t;



#endif // SH3_MODEL_HPP_INCLUDED
