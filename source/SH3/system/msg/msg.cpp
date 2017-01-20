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
#include "SH3/system/msg/msg.hpp"

/**
 *  Converts a SILENT HILL 3 message character into an ASCII character.
 *
 *  @param c The character we want to convert.
 */
static unsigned char SH3_2_ASCII(unsigned char c)
{
    return ((c) + 32);
}

typedef struct
{
    std::uint8_t c1;
    std::uint8_t c2;
} msg_attr_pair;

using namespace sh3_system;

void sh3_msg::Load(sh3_arc_vfile& file)
{
    //TODO: can we reserve() some space?

    msg_attr_pair c;
    sh3_arc_vfile::read_error e;

    while(file.ReadData(&c, sizeof(msg_attr_pair), e))
    {
        if(c.c1 == msg_ctrl::SH3_MSG_ATTR_START)
        {
            if(c.c2 == msg_ctrl::SH3_MSG_ATTR_START)
                break;

            currentAttr = (std::uint8_t)c.c2;
            continue;
        }

        msgString += SH3_2_ASCII(c.c1);
        msgString += SH3_2_ASCII(c.c2);
    }
}

