/** @file
 *
 *  Implementation of texture.hpp
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 2-1-2017
 *
 *  @author Jesse Buhagiar
 */
#include <SH3/graphics/texture.hpp>
#include <SH3/system/assert.hpp>
#include <SH3/system/log.hpp>
#include <SH3/arc/mft.hpp>
#include <SH3/arc/vfile.hpp>
#include <SH3/types/color.hpp>
#include "SH3/graphics/msbmp.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <limits>

using namespace sh3::graphics;

/** @defgroup graphics-types Graphics Types
 *  @{
*/

#pragma pack(push, 1)
struct tga_header
{
    static constexpr std::uint8_t TYPE_RGB24 = 2;
    static constexpr std::uint8_t FLAGS_FLIP = 0x20;

    std::uint8_t id_size = 0;               /**< Size of the ID field that follows this header (usually 0) */
    std::uint8_t colormap = 0;              /**< Is this image paletted (THIS MUST ALWAYS BE 0 FOR US!) */
    std::uint8_t type = TYPE_RGB24;         /**< Type of image (ALWAYS 2 FOR RGB24!) */
    std::uint8_t unused[5] = {};

    std::uint16_t x_origin = 0;             /**< Co-ordinate for first X value */
    std::uint16_t y_origin = 0;             /**< Co-ordinate for first Y value */
    std::uint16_t width = 0;
    std::uint16_t height = 0;
    std::uint8_t bpp = 24;                  /**< Bits per pixel */
    std::uint8_t flags = FLAGS_FLIP;
};
#pragma pack(pop)

/**@}*/

namespace
{
/**
 *  Dump a texture to a TARGA/TGA file
 *
 *  @param width - The Width of this texture.
 *  @param height - The Height of this texture.
 *  @param data - Pixel data
 *  @param bpp - Bitness of data (not the actual texture).
 */
void DumpRGB2Bitmap(std::uint32_t width, std::uint32_t height, std::vector<std::uint8_t>& data, std::uint8_t bpp)
{
    tga_header header;
    std::ofstream file("output.tga", std::ios::binary); // Open the stream for binary output

    ASSERT(width <= std::numeric_limits<decltype(header.width)>::max());
    ASSERT(height <= std::numeric_limits<decltype(header.height)>::max());

    header.width        = static_cast<std::uint16_t>(width);
    header.height       = static_cast<std::uint16_t>(height);

    switch(bpp)
    {
    case 16:
        header.bpp = 16;
        break;
    case 24:
        header.bpp = 24;
        break;
    case 32:
        header.bpp = 32;
        break;
    default:
        Log(LogLevel::WARN, "DumpRGB2Bitmap( ): Warning: Invalid bpp passed to function. Not writing image data.");
        return;
    }

    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    assert(data.size() <= std::numeric_limits<std::streamsize>::max());
    file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
}
}

//TODO: Scale the texture and then
void CTexture::Load(sh3::arc::mft& mft, const std::string& filename)
{
    sh3_texture_header          header;
    sh3::arc::vfile             file(mft, filename);
    sh3::arc::vfile::read_error e;
    std::vector<std::uint8_t>   data;       // Pixel data of this texture (with the header stripped)

    std::streamsize             offset = 0;

    std::size_t ret = file.ReadData(&header, sizeof(header), e);

    if(ret != sizeof(header))
        die("sh3_texture::Load( ): ReadData( ) != sizeof(header)!");

    // Check for the pesky 64-byte A7A7A7A7 header that sometimes precedes our texture header
    if(header.batchHeaderMarker == 0x00000000 && header.batchSize == 0xA7A7A7A7) // AHA!
    {
        offset = 0x40; // Skip the unknown header if it exists

        file.Seek(offset, std::ios_base::beg);
        file.ReadData(&header, sizeof(header), e);

        if(ret != sizeof(header))
            die("sh3_texture::Load( ): ReadData( ) != sizeof(header)!");
    }

    if(header.texSize == static_cast<decltype(header.texSize)>(header.texWidth * header.texHeight) * 4u)
    {
        header.bpp = 32; // Thanks KONAMI!
    }

    // Now that we're done that, we can check perform some sanity checks on our texture!
    if(header.texSize != static_cast<decltype(header.texSize)>(header.texWidth * header.texHeight * header.bpp) / 8u)
    {
        Log(LogLevel::WARN, "sh3_texture::Load( ): Warning, texSize != width * height * (bpp / 8)!");
        return; // TODO: Bind a color shader here
    }

    data.resize(header.texSize); // Early data resize (if it's an 8bpp texture, it will be resized anyway)

    width   = header.texWidth;
    height  = header.texHeight;
    bpp     = header.bpp;

    if(header.bpp == PixelFormat::PALETTE)
    {
        palette_info         pal_header;
        std::vector<rgba>    palette;    // Palette Data (I think this is BGRA)

        // First, we need to seek to the palette and read it in.
        file.Seek(offset + header.batchHeaderSize + header.texFileSize, std::ios_base::beg);
        file.ReadData(&pal_header, sizeof(pal_header), e);

        // Palette information is stored in blocks (usually of size 64-bytes). We also know how large the
        // palette is (in bytes, including padding between blocks). From this, we can deduce (with a bit of math)
        // that the whole palette occupies ~ paletteSize/entrySize bytes of space, contains a total of
        // entrySize/bypp colors per block, which therefore means we have a total of nBlocks * col_per_block colors,
        // which equates to about 256-colors in total (which seems accurate for an 8-bit texture).

        const std::size_t nBlocks = (pal_header.paletteSize / pal_header.entrySize) / pal_header.bytes_per_pixel;
        const std::size_t colorsPerBlock = pal_header.entrySize / pal_header.bytes_per_pixel;

        palette.resize(colorsPerBlock * nBlocks);

        for(std::size_t block = 0; block < nBlocks; ++block)
        {
            std::size_t read = file.ReadData(static_cast<void*>(&palette[block * colorsPerBlock]), pal_header.entrySize, e);

            if(read != pal_header.entrySize)
            {
                Log(LogLevel::WARN, "sh3_texture::Load( ): Warning: Number of bytes read in palette block != pal_header.entrySize! (Expected %lu, got %d)", read, pal_header.entrySize);
                break;
            }

            // Each block is 256 bytes.
            // Seek to next block.
            file.Seek(256 - pal_header.entrySize, std::ios_base::cur);
        }

        // we need to swap some colors every 32 pixels, starting from the 8th
        static constexpr std::size_t swapDistance = 32; // distance between swaps
        static constexpr std::size_t swapSize = 8; // amount of colors to swap

        if(palette.size() > 8)
        {
            for(auto iter = next(begin(palette), 8); static_cast<std::size_t>(distance(iter, end(palette))) > swapDistance; advance(iter, swapDistance))
            {
                // swap 8 colors
                const auto swapBlock = next(iter, swapSize);
                if(static_cast<std::size_t>(distance(swapBlock, end(palette))) < swapSize)
                {
                    Log(LogLevel::WARN, "Palette doesn't have enough colors left for swapping.");
                    break;
                }
                std::swap_ranges(iter, swapBlock, swapBlock);
            }
        }


        // Now that we've completely loaded the palette in its entirety, we can get the 8-bit index value
        // from the data section of the file and get it's color in the palette!

        //===---THIS IS A CLUSTER FUCK FOR NOW UNTIL WE UNDERSTAND HOW IN THE NAME OF CHRIST THIS WORKS---===//
        std::vector<std::uint8_t> iBuffer;  // Our index buffer that we put transformed indecies into

        data.resize(static_cast<std::size_t>(header.texWidth * header.texHeight) * 3u); // We strip the Alpha channel from the BGRA pixel beacuse it is hard locked to 0x80 (not 0xFF!!)
        iBuffer.resize(header.texSize);

        file.Seek(offset + (header.texFileSize - header.texSize), std::ios_base::beg); // Seek to the beginning of data

        if(header.texWidth > 96) // Apparently this is the distortion flag?!?!
        {
            if(header.texWidth % 16u != 0)
            {
                Log(LogLevel::WARN, "sh3_texture::Load( ): Warning: texWidth not divisible by 16!");
                header.texWidth = static_cast<decltype(header.texWidth)>(header.texWidth - header.texWidth % 16u);
            }

            if(header.texHeight % 4u != 0)
            {
                Log(LogLevel::WARN, "sh3_texture::Load ( ): Warning: texHeight not divisible by 4!");
                header.texHeight = static_cast<decltype(header.texHeight)>(header.texHeight - header.texHeight % 4u);
            }

            std::uint32_t x = 0;
            std::uint32_t y = 0;
            bool offsetFlipper = false;

            // We need to step through each index and unencrypt it
            // FIXME: distortion on 16-pixel wide block on the left
            while(true)
            {
                for(unsigned i = 0; i < 32; ++i)
                {
                    std::uint8_t index;
                    file.ReadData(&index, sizeof(index), e);

                    auto xoffset = static_cast<std::uint8_t>(((i << 2) & 0xfu) + ((i >> 2) & 0xfu));
                    if(i > 16 && i % 2u) // aka (i & 17) == 17
                    {
                        xoffset ^= 8u;
                        xoffset &= 0xfu;
                    }
                    if(offsetFlipper)
                    {
                        xoffset ^= 4u;
                    }

                    const auto tempx = x + xoffset; // - 16;
                    // every other pixel is for (y + 2)
                    const auto tempy = y + ((i % 2u) ? 2u : 0u);

                    iBuffer[(header.texWidth * tempy) + tempx % header.texWidth] = index;
                }

                x += 16;
                if(x < header.texWidth)
                {
                    continue;
                }

                x = 0;

                ++y;
                if(y % 2 == 0)
                {
                    // each iteration we read two lines: offset 0 and 2
                    // so after two iterations, we have read the lines with offset 0 and 2, 1 and 3
                    // so now we need to skip two additional lines, since we read 2 and 3 already
                    y += 2;

                    if(y >= header.texHeight)
                    {
                        Log(LogLevel::WARN, "sh3_texture::Load( ): Warning: y <= header.texHeight!");
                        break;
                    }

                    offsetFlipper = !offsetFlipper;
                }

                if(y == header.texHeight)
                {
                    break;
                }
            }

            // Christ help me...
            for(std::size_t pindex = 0, doff = 0; pindex < iBuffer.size() && doff < data.size(); pindex++, doff += 3)
            {
                rgba pixel = palette[iBuffer[pindex]];

                data[doff + 0]  = pixel.r;
                data[doff + 1]  = pixel.g;
                data[doff + 2]  = pixel.b;
            }
        }
        else // If the distortion flag isn't set, just read the pixel data in from the palette.
        {
            for(std::size_t i = 0; i < static_cast<std::size_t>(header.texWidth * header.texHeight) * 3u; i += 3)
            {
                std::uint8_t index;
                file.ReadData(&index, sizeof(index), e);
                rgba pixel = palette[index];

                data[i + 0]   = pixel.r;
                data[i + 1]   = pixel.g;
                data[i + 2]   = pixel.b;
            }
        }

        DumpRGB2Bitmap(header.texWidth, header.texHeight, data, 24);
    }
    else if(header.bpp == PixelFormat::RGBA)
    {
        file.ReadData(&data[0], header.texSize, e);
        DumpRGB2Bitmap(header.texWidth, header.texHeight, data, 32);
    }
    else if(header.bpp == PixelFormat::BGR)
    {
        file.ReadData(&data[0], header.texSize, e);
        DumpRGB2Bitmap(header.texWidth, header.texHeight, data, 24); // Output will be reversed!
    }
    else if(header.bpp == PixelFormat::RGBA16)
    {
        //TODO: Some kind of fucked up shit here. I think this is R5G5B5A1 or something like that..
        file.ReadData(&data[0], header.texSize, e);
        DumpRGB2Bitmap(header.texWidth, header.texHeight, data, 16);
    }
    else
    {
        die("sh3_texture::Load( ): Unknown Pixel Format, %d", header.bpp);
    }


    glGenTextures(1, &tex);             // Create a texture
    glBindTexture(GL_TEXTURE_2D, tex);  // Bind it for use

    GLenum srcFormat;
    GLint dstFormat;
    GLenum type;

    // Create the texture according to its pixel format!
    switch(header.bpp)
    {
        case PixelFormat::RGBA:     // Regular 32-bit RGBA
            srcFormat = GL_RGBA;
            dstFormat = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
        case PixelFormat::BGR:      // 24-bit BGR
            srcFormat = GL_BGR;
            dstFormat = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
        case PixelFormat::RGBA16:   // 16-bit RGBA. OpenGL supports this (I think)
            srcFormat = GL_RGBA;
            dstFormat = GL_RGBA;
            type = GL_UNSIGNED_SHORT_5_5_5_1;
            break;
        case PixelFormat::PALETTE:
            srcFormat = GL_RGB;
            dstFormat = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
        default:
            die("sh3_texture::Load( ): Invalid pixel format: %d", header.bpp);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, dstFormat, header.texWidth, header.texHeight, 0, srcFormat, type, data.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear interpolation for the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0); // Un-bind this texture.
}

void CTexture::Load(const std::string& path)
{
    using namespace sh3::graphics::bmp;

    std::ifstream   file;
    file_header     fheader;    // File information header
    info_header     iheader;
    std::uint16_t   bpp;        // Bitmap bit depth

    std::vector<palette_color> palette;     // Vector containing colour data for this image
    std::vector<std::uint8_t> data;                // Vector containing image data or palette offsets

    file.open(path, std::ios::binary);

    if(!file.is_open())
    {
        // If this happens we're in a bit of trouble, because we can't load the default texture..... We could crash here.
        Log(LogLevel::ERROR, "msbmp::Load( ): Unable to open a handle to %s!", path.c_str());
        return;
    }

    file.read(reinterpret_cast<char*>(&fheader), sizeof(fheader)); // Read in the file header
    if(fheader.tag[0] != 'B' || fheader.tag[1] != 'M')
    {
        Log(LogLevel::WARN, "msmbmp::Load( ): Warning: This file does not appear to be a BMP! Reason: BAD_HEADER!");
        return;
    }

    file.read(reinterpret_cast<char*>(&iheader), sizeof(iheader)); // Read in the information header
    ASSERT(iheader.width >= 0);
    width = static_cast<GLsizei>(iheader.width);
    ASSERT(iheader.height >= 0);
    height = static_cast<GLsizei>(iheader.height);
    bpp = iheader.bpp;

    data.resize(static_cast<std::size_t>(width * height) * 3u); // Some programs misformat this, so calculate it ourselves. This is usually 24-bit even for 8-bit paletted images!

    // We can now differentiate if our bitmap is 8bpp or 24bpp
    if(bpp == 8) // Paletted images are for fuckwits, I'm looking at you, Konami....
    {
        /**
        palette.resize(iheader.palette_size);
        file.seekg(sizeof(fheader) + sizeof(iheader), std::ios_base::beg); // Seek to the palette
        file.read(reinterpret_cast<char*>(palette.data()), iheader.palette_size);
        file.seekg(fheader.pix_offset, std::ios_base::beg); // Ensure we are actually in the data section

        // Slightly expensive, but we now copy each pixel into the data buffer by getting it's index in the palette.
        // Seeing as this is done once, I doubt it matters too much, especially if we do this during load time
        // and keep the image alive for the whole of the scene duration (there are 3 bmps total in SH3, not including
        // our default 'error' texture)
        for(pixel& p: data)
        {
            palette_color color;    // 4-byte colour, stored in BGRA (where A is unused)
            std::uint8_t index;     // Image byte, which is the index into the colour palette
            file.read(reinterpret_cast<char*>(&index), sizeof(index));

            color = palette[index];
            p.r = color.r; // Extract Red
            p.g = color.g; // Extract Green
            p.b = color.b; // Extract Blue
        }
        **/
        Log(LogLevel::WARN, "CTexture::Load() does not support 8-bit MS bitmaps! Consider revising!");
        return;
    }
    if(bpp == 24)
    {
        file.seekg(fheader.pix_offset, std::ios_base::beg); // Seek to the image data
        ASSERT(data.size() <= std::numeric_limits<std::streamsize>::max());
        //file.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size())); // Read in the image data
        file.read(reinterpret_cast<char*>(data.data()), data.size());
    }
    else
    {
        Log(LogLevel::WARN, "msbmp::Load( ): Bad pixel depth %d!", bpp);
        return;
    }

    /**
     * They're actually mirrored too! The co-ordinate systems are COMPLETELY different,
     * OpenGL is bottom left, bmp is (more pracitically), top left. We should probably pass in
     * some kind of shader attribute (via a boolean) so that we can swap the <s, t> co-ords.
     */
    //std::reverse(data.begin(), data.end()); // Reverse the data because .bmp files are actually upside down in RAM

    // Do the actual texture upload
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear interpolation for the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0); // Un-bind this texture.
}

void CTexture::Bind(GLenum textureUnit)
{
    ASSERT(textureUnit >= GL_TEXTURE0 && textureUnit <= GL_TEXTURE31);

    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, tex);
}

void CTexture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

