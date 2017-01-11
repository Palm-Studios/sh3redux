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
 *  @date 2/1/2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef SH3_TEXTURE_HPP_INCLUDED
#define SH3_TEXTURE_HPP_INCLUDED

#include <SH3/arc/types.hpp>

#include <cstdint>
#include <limits>
#include <vector>
#include <GL/gl.h>

#include "SH3/error.hpp"

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
    typedef struct
    {
        std::uint32_t batchHeaderMarker;   /**< This should be 0xFFFFFFFF to mark a header chunk */
        std::uint8_t  unused1[4];          /**< There are a lot of unused DWORDs, I assume to align everything nicely */
        std::uint32_t batchHeaderSize;     /**< Size of the first part of the whole header */
        std::uint32_t batchSize;           /**< = res * res * (bpp/8) * #tex + 128 * #tex */
        std::uint8_t  unused2[4];          /**< Unused **/
        std::uint32_t numBatchedTextures;  /**< Number of textures in this texture file */
        std::uint8_t  unused3[8];          /**< Unused **/
        std::uint32_t texHeaderSegMarker;  /**< Secondary texture marker. This signifies the start of the texture information header. This is also 0xFFFFFFFF */
        std::uint8_t  unused4[4];          /**< Unused **/
        std::uint16_t texWidth;            /**< The width of this texture */
        std::uint16_t texHeight;           /**< The height of this texture */
        std::uint8_t  bpp;                 /**< Number of bits per pixel of this texture. NOTE: 8bpp is believed to be paletted! */
        std::uint8_t  dataOffset;          /**< #bytes from texHeaderSize+16 to 128 (0 filled) */
        std::uint8_t  unused5[4];          /**< Unused **/
        std::uint32_t texSize;             /**< The size of this texture in bytes (w * h * [bpp/8]) */
        std::uint32_t texHeaderSize;       /**< = texSize + texHeaderSize + 16 + endFilleSize */
        std::uint8_t  unused6[4];          /**< Unused **/
        std::uint32_t unknown1;            /**< Completely unknown, probably unimportant for now */
        std::uint32_t unknown2;            /**< Usually 1! */
        std::uint32_t unused7[15];         /**< Unused **/
    } sh3_texture_header;

    /**@}*/

    /**
     *
     * Describes a logical texture that can be bound to OpenGL
     *
     * Defines a few bits of data and some functions to load in a texture
     * from a SILENT HILL 3 .arc section.both batch and individual texture
     *
     */
    class texture
    {
    public:
        /**
         *  Failure codes for @ref Load.
         */
        enum class load_result
        {
            SUCCESS,
            INVALID_CHUNK,
            END_OF_FILE,
        };

        struct load_error final : public error<load_result>
        {
        public:
            std::string message() const;
        };

        texture(){}
        texture(const std::string& filename, sh3_arc& arc){Load(filename, arc);}

        /**
         * Load a texture and return a result to it
         *
         * @param filename - Virtual file path to file.
         * @param arc - Reference to sh3_arc (arc.arc)
         *
         * @return @ref load_result indicating whether the read failed or not
         *
         */
        load_result Load(const std::string& filename, sh3_arc& arc);

    private:
        static constexpr std::uint32_t INVALID_TEXTURE = std::numeric_limits<uint32_t>::max();

        std::vector<std::uint8_t> pixbuff;
        std::string               texpath;
        std::uint32_t             texid = INVALID_TEXTURE;

        sh3_texture_header texheader = {};
    };
}

#endif // SH3_TEXTURE_HPP_INCLUDED
