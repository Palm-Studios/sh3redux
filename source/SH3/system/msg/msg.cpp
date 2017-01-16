/** @file
 *  Implementation of msg.hpp
 *
 *
 *  @copyright 2016-2017  Palm Studios
 *
 *  @date 16-1-2017
 *
 *  @author Jesse Buhagiar
 */
#include "SH3/SH3/system/msg/msg.hpp"

using namespace sh3_system;

void sh3_msg::Load(sh3_arc_vfile& file)
{

    std::uint8_t c;
    std::uint8_t c2;
    sh3_arc_vfile::read_error e;

    // While we are reading a character
    while(file.ReadData(&c, 1, e))
    {
        file.ReadData(&c2, 1, e); // Read in a second character (attribute/terminator check)

        if(c == SH3_ATTR_START && c2 == SH3_ATTR_START) // If BOTH characters == 0xFF, this message has been fully copied.
            break;

        if(c == SH3_ATTR_START)
        {
            currentAttr = (std::uint8_t)c2; // Convert this character to it's actual attribute
            continue; // Don't add c or c2 to the string, it's an attribute
        }

        msgString += c;
        msgString += c2
    }
}

