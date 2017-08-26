/** @file
 *
 *  Functions and structures to load a texture from an arc section
 *
 *  @copyright 2016  Palm Studios, Mike Meinhardt and de_lof
 *
 *  @note   I'd like to thank Mike for all this, he put a lot of time into reverse engineering
 *          all of the file types, including all of the textures, meshes and even the motion capture
 *          skeletal animation that Konami captured. Thanks mate!
 *
 *  @note It would seem the 8-bit texture palette comes at the END of the texture, not at beginning like one would expect.
 *  @note bpp == 32, RGBA; bpp == 24, BGR; bpp == 16, RGBA16; bpp=8, Paletted.
 *
 *  @date 2-1-2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef SH3_TEXTURE_HPP_INCLUDED
#define SH3_TEXTURE_HPP_INCLUDED

#include "SH3/arc/vfile.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

namespace sh3 { namespace arc {
    struct mft;
} }

namespace sh3_graphics
{
    /** @defgroup graphics-headers Graphics headers
     *  @{
     */

    #pragma pack(push, 1)

    /**
     *  Header that comes after the batch header. Contains information about the texture.
     */
    struct sh3_texture_info_header
    {
        std::uint32_t texHeaderMarker;      /**< Magic Number. This is ALWAYS 0xFFFFFFFF */
        std::uint32_t unused1;               /**< Unused 32-bit value. Apparently for format identification. */
        std::uint16_t width;                /**< The hidth of this texture */
        std::uint16_t height;               /**< The height of this texture */
        std::uint8_t  bpp;                  /**< Number of bits per pixel of this texture. NOTE: 8bpp is paletted! */
        std::uint8_t  dataOffset;           /**< #bytes from texHeaderSize+16 to 128 (0 filled) */
        std::uint16_t padding;              /**< Possibly padding, as it's usually 0 */
        std::uint32_t texSize;              /**< The size of this texture in bytes (w * h * [bpp/8]) */
        std::uint32_t texFileSize;          /**< = texSize + texHeaderSize + 16 + endFilleSize */
        std::uint32_t unknown;              /**< This is unknown/unused */
        std::uint8_t  widthattrib;          /**< 256 - 8; 32 - 5; 1024 - A; 512 - 9 (I have no clue what this is...) */
        std::uint8_t  heightattrib;         /**< Same deal */
        std::uint16_t magic;                /**< Always 0x9999 */
    };

    /**
     * Full texture header.
     *
     * Both batch and individual texture.
     */
    struct sh3_texture_header
    {
        std::uint32_t batchHeaderMarker;    /**< This should be 0xFFFFFFFF to mark a header chunk */
        std::uint8_t  unused1[4];           /**< There are a lot of unused DWORDs, I assume to align everything nicely */
        std::uint32_t batchHeaderSize;      /**< Size of the first part of the whole header */
        std::uint32_t batchSize;            /**< = res * res * (bpp/8) * #tex + 128 * #tex */
        std::uint8_t  unused2[4];           /**< Unused **/
        std::uint32_t numBatchedTextures;   /**< Number of textures in this texture file */
        std::uint8_t  unused3[8];           /**< Unused **/
        std::uint32_t texHeaderSegMarker;   /**< Secondary texture marker. This signifies the start of the texture information header. This is also 0xFFFFFFFF */
        std::uint8_t  unused4[4];           /**< Unused **/
        std::uint16_t texWidth;             /**< The width of this texture */
        std::uint16_t texHeight;            /**< The height of this texture */
        std::uint8_t  bpp;                  /**< Number of bits per pixel of this texture. NOTE: 8bpp is believed to be paletted! */
        std::uint8_t  dataOffset;           /**< #bytes from texHeaderSize+16 to 128 (0 filled) */
        std::uint8_t  unused5[2];           /**< Unused **/
        std::uint32_t texSize;              /**< The size of this texture in bytes (w * h * [bpp/8]) */
        std::uint32_t texFileSize;          /**< = texSize + texHeaderSize + 16 + endFilleSize */
        std::uint8_t  unused6[4];           /**< Unused **/
        std::uint32_t unknown1;             /**< Completely unknown, probably unimportant for now */
        std::uint32_t unknown2;             /**< Usually 1! */
        std::uint32_t unused7[15];          /**< Unused **/
    };

    /**
     *  Color palette structure. Contains information about the colour palette.
     */
    struct palette_info
    {
        std::uint32_t paletteSize;      /**< Number of colors (??) in our color palette */
        std::uint32_t unused1[2];       /**< Unused as far as I can tell */
        std::uint8_t  bytes_per_pixel;  /**< Number of bytes per pixel */
        std::uint8_t  unused2;          /**< Blank Byte */
        std::uint8_t  entrySize;        /**< Size of one color block in this palette */
        std::uint8_t  unknown[17];      /**< Unknown or unused bytes */
        std::uint8_t  distortion;       /**< I have no clue what this is, but it could be important */
        std::uint8_t  pad[15];          /**< These are all zero. Here so we can align to the palette after a read */
    };

    #pragma pack(pop)

    /**@}*/

    /**
     *
     * Describes a logical texture that can be bound to OpenGL
     *
     * Defines a few bits of data and some functions to load in a texture
     * from a SILENT HILL 3 .arc section.both batch and individual texture
     *
     */
    struct sh3_texture final
    {
    public:

        enum PixelFormat
        {
            RGBA    = 32,
            BGR     = 24,
            RGBA16  = 16,
            PALETTE = 8,
        };

        sh3_texture(sh3::arc::mft& mft, const std::string& filename){Load(mft, filename);}
        ~sh3_texture(){}

        /**
         *  Loads a texture from a Virtual File and creates a logical texture
         *  on the gpu
         *
         *  @note Should we scale this ala SILENT HILL 3's "Interal Render Resolution"???
         */
        void Load(sh3::arc::mft& mft, const std::string& filename);

        /**
         *  Bind this texture for use with any draw calls
         *
         *  @param textureUnit The texture unit we want to bind this texture to
         */
        void Bind(GLenum textureUnit);

         /**
          * Unbind this texture from the
          */
         void Unbind();

    private:
        GLuint tex;                         /**< ID representing this texture */
    };
}

#endif // SH3_TEXTURE_HPP_INCLUDED
