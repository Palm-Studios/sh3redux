/*++

Copyright (c) 2016  Palm Studios and Mike M (@perdedork)

Module Name:
        sh3_texture.hpp

Abstract:
        Defines data structures to load an manipulate a texture

Author:
        Jesse Buhagiar

Environment:

Notes:
        I'd like to thank Mike for all this, he put a lot of time into reverse engineering
        all of the file types, including all of the textures, meshes and even the motion capture
        skeletal animation that Konami captured. Thanks mate!

Revision History:
        1-1-2017: File Created                              [jbuhagiar]

--*/
#ifndef SH3_TEXTURE_H_INCLUDED
#define SH3_TEXTURE_H_INCLUDED

#include <cstdint>

typedef struct
{
    uint32_t    batchHeaderMarker;          // This should be 0xFFFFFFFF to mark a header chunk
    uint32_t    unused1;                    // There are a lot of unused DWORDs, I assume to align everything nicely
    uint32_t    batchHeaderSize;            // Size of the first part of the whole header
    uint32_t    batchSize;                  // = res * res * (bpp/8) * #tex + 128 * #tex
    uint32_t    unused2;
    uint32_t    numBatchedTextures;         // Number of textures in this texture file
    uint32_t    unused3[2];
    uint32_t    texHeaderSegMarker;         // Secondary texture marker. This signifies the start of the texture information header
    uint32_t    unused4;
    uint16_t    texWidth;                   // The width of this texture
    uint16_t    texHeight;                  // The height of this texture;
    uint8_t     bpp;                        // Number of bits per pixel of this texture. NOTE: 8bpp is believed to be paletted!
    uint8_t     dataOffset;                 // #bytes from texHeaderSize+16 to 128 (0 filled)
    uint16_t    unused5;
    uint32_t    texSize;                    // The size of this texture in bytes (w * h * [bpp/8])
    uint32_t    texHeaderSize;              // = texSize + texHeaderSize + 16 + endFilleSize
    uint32_t    unused6;
    uint32_t    unknown1;                   // Completely unknown, probably unimportant for now
    uint32_t    unknown2;                   // Usually 1!
    uint32_t    unused7[15];
} sh3_texture_header;



#endif // SH3_TEXTURE_H_INCLUDED
