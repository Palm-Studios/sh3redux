/** @file
 *
 *  Implementation of texture.hpp
 *
 *  @copyright 2016  Palm Studios
 *
 *  @note
 *
 *  @date 2-1-2017
 *
 *  @author Jesse Buhagiar
 */
#include <SH3/graphics/texture.hpp>
#include <SH3/system/log.hpp>

using namespace sh3_graphics;

texture::load_result sh3_graphics::texture::Load(const std::string& filename, sh3_arc& arc)
{
    if(arc.LoadFile(filename, pixbuff) == ARC_FILE_NOT_FOUND)
    {
        die("sh3_graphics::texture::Load( ): Unable to find file %s!", filename.c_str());
    }

    return load_result::TEOF;
}