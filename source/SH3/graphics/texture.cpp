/** @file
 *
 *  Functions and structures to load a texture from an arc section
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note   I'd like to thank Mike for all this, he put a lot of time into reverse engineering
            all of the file types, including all of the textures, meshes and even the motion capture
            skeletal animation that Konami captured. Thanks mate!
 *
 *  \date 2/1/2017
 *
 *  \author Jesse Buhagiar
 */
#ifndef SH3_TEXTURE_H_INCLUDED
#define SH3_TEXTURE_H_INCLUDED

#include <cstdint>
#include <SH3/arc/types.hpp>
#include <GL/gl.h>

namespace sh3_graphics
{
    /**
     * Full texture header (both batch and individual texture).
     */
    typedef struct
    {
        std::uint32_t batchHeaderMarker;          // This should be 0xFFFFFFFF to mark a header chunk
        std::uint32_t unused1;                    // There are a lot of unused DWORDs, I assume to align everything nicely
        std::uint32_t batchHeaderSize;            // Size of the first part of the whole header
        std::uint32_t batchSize;                  // = res * res * (bpp/8) * #tex + 128 * #tex
        std::uint32_t unused2;
        std::uint32_t numBatchedTextures;         // Number of textures in this texture file
        std::uint32_t unused3[2];
        std::uint32_t texHeaderSegMarker;         // Secondary texture marker. This signifies the start of the texture information header
        std::uint32_t unused4;
        std::uint16_t texWidth;                   // The width of this texture
        std::uint16_t texHeight;                  // The height of this texture;
        std::uint8_t  bpp;                        // Number of bits per pixel of this texture. NOTE: 8bpp is believed to be paletted!
        std::uint8_t  dataOffset;                 // #bytes from texHeaderSize+16 to 128 (0 filled)
        std::uint16_t unused5;
        std::uint32_t texSize;                    // The size of this texture in bytes (w * h * [bpp/8])
        std::uint32_t texHeaderSize;              // = texSize + texHeaderSize + 16 + endFilleSize
        std::uint32_t unused6;
        std::uint32_t unknown1;                   // Completely unknown, probably unimportant for now
        std::uint32_t unknown2;                   // Usually 1!
        std::uint32_t unused7[15];
    } sh3_texture_header;

    /**
     * \class texture
     *
     * \brief Describes a logical texture and files to
     *
     * \note
     *
     * Defines a few bits of data and some functions to load in a texture
     * from a SILENT HILL 3 .arc section.
     */
    class texture
    {
    public:
    enum class result
    {
        INVALID_CHUNK,
        TEOF,
    };

        texture(std::string& filename){Load(filename);};
        ~texture(){delete[] pixbuff;}

        GLuint Load(std::string& path);

    private:
        std::uint8_t*   pixbuff;
        std::string     texname;
    };


}
#endif // SH3_TEXTURE_H_INCLUDED
