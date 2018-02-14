/** @file
 *  Macros and structures related to pixel information.
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 14-12-2016
 *
 *  @author Jesse Buhagiar
 */
#ifndef COLOR_HPP_INCLUDED
#define COLOR_HPP_INCLUDED

#include <GL/gl.h>
#include <cstdint>
#include <cstddef>

/** @defgroup graphics-types Graphics Types
 *  @{
*/

/**
 *  RGBA pixel
 */
struct rgba
{
    std::uint8_t r, g, b, a;
};

/*
 *  RGBA16 pixel
 */
struct rgba16
{
    std::uint8_t r : 5;
    std::uint8_t g : 5;
    std::uint8_t b : 5;
    std::uint8_t a : 1;
};

/**
 *  RGB24 pixel
 */
struct rgb
{
    std::uint8_t r, g, b;
};

/**
 * BGRA pixel (swizzled RGBA)
 */
struct bgra
{
    std::uint8_t b, g, r, a;
};

/**
 *  BGR24 pixel (swizzled RGB24)
 */
struct bgr
{
    std::uint8_t b, g, r;
};

static_assert(offsetof(struct rgb, b) == 2* sizeof(std::uint8_t), "struct has been padded on this compiler! (mingw/win32 use '-mno-ms-bitfields!')");

/**@}*/

#endif // COLOR_HPP_INCLUDED
