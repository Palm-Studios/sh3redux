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
#include <SH3/system/log.hpp>
#include <SH3/arc/vfile.hpp>

#include <cassert>


using namespace sh3_graphics;

//TODO: Scale the texture and then
void sh3_texture::Load(sh3_arc& mft, const std::string& filename)
{
    sh3_texture_header          header;
    sh3_arc_vfile               file(mft, filename);
    sh3_arc_vfile::read_error   e;
    std::vector<std::uint8_t>   data;     // Pixel data of this texture (with the header stripped)

    std::size_t ret = file.ReadData(&header, sizeof(header), e); // Read in the header

    if(ret != sizeof(header))
        die("sh3_texture::Load( ): Header only read %d bytes of data! It should have been %d", ret, sizeof(header)); // TODO: Fallback on dummy texture (like Source's purple and black checkboard)

    if(header.bpp == 8)
        Log(LogLevel::WARN, "WARNING: 8-bit textures are currently unsupported! They WILL look like nothing but pixel garbage.");

    Log(LogLevel::INFO, "Texture info:\n\ttexWidth:%d\n\ttexHeight:%d\n\ttexSize:%d\n\tbpp:%d", header.texWidth, header.texHeight, header.texSize, header.bpp);

    file.Seek(sizeof(header) + header.dataOffset, std::ios_base::beg);

    data.resize(header.texSize);

    if(header.texSize != header.texWidth * header.texHeight * header.bpp)
    {
        Log(LogLevel::ERROR, "sh3_texture::Load( ): texWidth * texHeight * bpp != header.texSize!");
        return;
    }

    ret = file.ReadData(&data[0], header.texSize, e);
    if(ret != header.texSize)
        Log(LogLevel::WARN, "sh3_texture::Load( ): Only read %d out of %d bytes!", ret, header.texSize);

    // Now actually create a logical texture with OpenGL on the gpu
    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, header.texWidth, header.texHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, &data[0]);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind this texture
}

void sh3_texture::Bind(GLenum textureUnit)
{
    assert(textureUnit >= GL_TEXTURE0 && textureUnit <= GL_TEXTURE31);

    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, tex);
}

void sh3_texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

