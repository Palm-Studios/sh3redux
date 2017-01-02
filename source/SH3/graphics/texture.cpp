/** @file
 *
 *  Implementation of texture.hpp
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note
 *
 *  \date 2/1/2017
 *
 *  \author Jesse Buhagiar
 */
#include <SH3/graphics/texture.hpp>
#include <SH3/system/log.hpp>

using namespace sh3_graphics;

/**
 * \fn Load
 *
 * \brief Load a texture and return a result to it
 *
 * \note
 *
 * \return \c result indicating whether the read failed or not
 *
 */
texture::result sh3_graphics::texture::Load(const std::string& filename)
{
    if(arc.LoadFile(filename, pixbuff) == ARC_FILE_NOT_FOUND)
    {
        die("sh3_graphics::texture::Load( ): Unable to find file %s!", filename.c_str());
    }

    return result::TEOF;
}
