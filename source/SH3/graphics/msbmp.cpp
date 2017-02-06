/** @file
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 6-2-17
 *
 *  @author Jesse Buhagiar
 */

#include "SH3/graphics/msbmp.hpp"
#include "SH3/system/log.hpp"

#include <fstream>
#include <algorithm>


struct pixel
{
    std::uint8_t r, g, b;
};

struct palette_color
{
    std::uint8_t b, g, r, a;
};

using namespace sh3_graphics;

msbmp::load_result msbmp::Load(const std::string& path)
{
    std::ifstream   file;
    bmp_file_header fheader;    // File information header
    bmp_info_header iheader;
    load_result     res;
    std::uint16_t   bpp;        // Bitmap bit depth

    std::vector<palette_color> palette;     // Vector containing colour data for this image
    std::vector<pixel> data;                // Vector containing image data or palette offsets

    res.set_error(result::BAD_LOAD);
    file.open(path, std::ios::binary);

    if(!file.is_open())
    {
        // If this happens we're in a bit of trouble, because we can't load the default texture..... We could crash here.
        Log(LogLevel::ERROR, "msbmp::Load( ): Unable to open a handle to %s!", path.c_str());
        return res;
    }

    file.read(reinterpret_cast<char*>(&fheader), sizeof(fheader)); // Read in the file header
    if(fheader.tag[0] != 'B' || fheader.tag[1] != 'M')
    {
        Log(LogLevel::WARN, "msmbmp::Load( ): Warning: This file does not appear to be a BMP! Reason: BAD_HEADER!");
        return res;
    }

    file.read(reinterpret_cast<char*>(&iheader), sizeof(iheader)); // Read in the information header
    width = iheader.width;
    height = iheader.height;
    bpp = iheader.bpp;

    data.resize(width * height * 3); // Some programs misformat this, so calculate it ourselves. This is usually 24-bit even for 8-bit paletted images!

    // We can now differentiate if our bitmap is 8bpp or 24bpp
    if(bpp == 8) // Paletted images are for fuckwits, I'm looking at you, Konami....
    {
        palette.resize(iheader.palette_size);
        file.seekg(sizeof(fheader) + sizeof(iheader), std::ios_base::beg); // Seek to the palette
        file.read((char*)&palette[0], iheader.palette_size);
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

    }
    else if(bpp == 24)
    {
        file.seekg(fheader.pix_offset, std::ios_base::beg); // Seek to the image data
        file.read((char*)&data[0], data.size()); // Read in the image data
    }
    else
    {
        Log(LogLevel::WARN, "msbmp::Load( ): Bad pixel depth %d!", bpp);
        return res;
    }

    std::reverse(data.begin(), data.end());

    glGenTextures(1, &tex); // Bind texture for use
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

    res.set_error(result::SUCCESS);
    return res;
}

void msbmp::Draw()
{

}