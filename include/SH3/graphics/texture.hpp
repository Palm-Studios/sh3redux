/** @file
 *
 *  Functions and structures to load a texture from an arc section
 *
 *  @copyright 2016  Palm Studios and Mike M
 *
 *  @note   I'd like to thank Mike for all this, he put a lot of time into reverse engineering
            all of the file types, including all of the textures, meshes and even the motion capture
            skeletal animation that Konami captured. Thanks mate!
 *
 *  @note 8-bit textures are currently unsupported, as they are palletted. See <a href=https://www.reddit.com/r/REGames/comments/5kp62x/help_reverse_engineering_silent_hill_3_texture/?st=iyn04asn&sh=92f998d9>here</a>
 *
 *  @date 2-1-2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef SH3_TEXTURE_HPP_INCLUDED
#define SH3_TEXTURE_HPP_INCLUDED

#include "SH3/arc/types.hpp"
#include "SH3/arc/vfile.hpp"

#include <GL/glew.h>
#include <GL/gl.h>


namespace sh3_graphics
{
    /** @defgroup graphics-headers Graphics headers
     *  @{
     */

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
        sh3_texture(sh3_arc& mft, const std::string& filename){Load(mft, filename);}
        ~sh3_texture(){}

        /**
         *  Loads a texture from a Virtual File and creates a logical texture
         *  on the gpu
         *
         *  @note Should we scale this ala SILENT HILL 3's "Interal Render Resolution"???
         */
        void Load(sh3_arc& mft, const std::string& filename);

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

        std::vector<std::uint8_t> data;     /**< Pixel data of this texture (with the header stripped) */
    };
}

#endif // SH3_TEXTURE_HPP_INCLUDED
