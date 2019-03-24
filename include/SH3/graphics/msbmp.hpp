/** @file
 *
 *  Class and data structures to load Microsoft BMP/DIB files.
 *
 *  @copyright 2016-2017  Palm Studios
 *
 *  @date 6-2-2017
 *
 *  @note All paths specified by the constructor are PHYSICAL paths on disk.
 *  @note The palette data is stored as BGR with the last byte being 0xFF (unused)
 *  @note The image data is actually stored upside down (reflected along the x axis).
 *  @note This class is a container for Bitmap loading and Data. All texture calls are done via @ref sh3::graphics::CTexture
 *
 *  @author Jesse Buhagiar
 */
#ifndef MSBMP_HPP_INCLUDED
#define MSBMP_HPP_INCLUDED

#include "SH3/error.hpp"

#include <cstdint>
#include <vector>
#include <string>

#include <GL/gl.h>

namespace sh3 { namespace graphics {

namespace bmp {
/** @defgroup graphics-headers Graphics headers
 *  @{
 */

 #pragma pack(push, 1) // Stop the compiler from giving us stupid naturally aligned boundaries (__attribute__((packed)) didn't work...)

/**
 * Bitmap file information header.
 */
struct file_header
{
    char tag[2];                /**< This is always the ASCII characters "BM" */
    std::uint32_t fsize;        /**< The size of this file in bytes */
    std::uint16_t rsvd1;        /**< 2 reserved bytes. Determined by program that created this bmp */
    std::uint16_t rsvd2;        /**< 2 reserved bytes. Determined by program that created this bmp */
    std::uint32_t pix_offset;   /**< Offset to beginning of image data */
};

/**
 * Bitmap information header. Contains information about the actual bitmap (such as width and height)
 */
struct info_header
{
    std::uint32_t header_size;  /**< Size of the header following this one. Used to determine which one to use. */
    std::int32_t width;         /**< Width of this image. For some reason it's a SIGNED 32-bit integer.... */
    std::int32_t height;        /**< Height of this image. */
    std::uint16_t color_plane;  /**< Number of colour planes. Should be 1. */
    std::uint16_t bpp;          /**< Number of bits per pixel (for us, should be 8 or 24) */
    std::uint32_t compression;  /**< Compression method used. THIS SHOULD ALWAYS BE 0 FOR UNCOMPRESSED!!!! */
    std::uint32_t image_size;   /**< Size of this image in bytes */
    std::int32_t x_res;         /**< Horizontal resolution in pixels per metre */
    std::int32_t y_res;         /**< Vertical resolution in pixels per metre */
    std::uint32_t palette_size; /**< Number of colours in the colour palette (NOT GIVEN IN BYTES!!!) */
    std::uint32_t imp_colors;   /**< Number of important colours used. 0 when every colour is important. Usually ignored */
};

struct pixel
{
    std::uint8_t r, g, b;
};

struct palette_color
{
    std::uint8_t b, g, r, a;
};

#pragma pack(pop)

}}}

/**@}*/


#endif // MSBMP_HPP_INCLUDED
