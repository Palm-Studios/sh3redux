/** @file
 *  Data structure representing a SILENT HILL 3 message.
 *  These messages (and their attributes) can later be passed off
 *  to the renderer to be drawn as text strings with FTGL.
 *  The font used by SILENT HILL 3 is VERY similar to Times New Roman, however as of now,
 *  we currently don't know where the font actually resides (it's not in the MFT).
 *
 *  How this (hopefully) will work:
 *  The string to print is printed character (slow? Shouldn't be imho).
 *  During each time we draw, we check whether or not we need to change
 *  the attribute that we are drawing with (effect, colour etc). The current attribute
 *  to use is determined by scanning the current character we want to print for 0xFF.
 *  This character is known as the @ref SH3_ATTR_START character, and signals to the message
 *  manager that the next character read will be an attribute byte.
 *
 *  On entering a 'scene' (level\area), all messages are loaded. by the message manager
 *
 *  @note Most, if not all strings in SILENT HILL 3 are centered.
 *
 *  @copyright 2016-2017  Palm Studios
 *
 *  @date 12-1-2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef MSG_HPP_INCLUDED
#define MSG_HPP_INCLUDED

#include <cstdint>

#define SH3_2_ASCII(x) ((x) - 32) /**< Turns a SILENT HILL 3 msg character into an ASCII character */

#define SH3_MSG_BARS        0xFA /**< Tells the font manager to draw 3 lines of black bars across the screen (???) */
#define SH3_MSG_BREATHE     0xFB /**< Tells the font manager to breathe the text in and out from white to red and back again */
#define SH3_MSG_NEWLINE     0xFD /**< Tells the font manager to draw a newline for the next part of this message */
#define SH3_ATTR_START      0xFF /**< Signals that the next byte will be a text attribute */

#define SH3_MSG_WHITECLR    0x00 /**< Tells the font manager to draw this message in white or transparent (?) */
#define SH3_MSG_GREEN       0x01 /**< Tells the font manager to draw this message in cyan */
#define SH3_MSG_RED         0x02 /**< Tells the font manager to draw this message in red */
#define SH3_MSG_GREEN       0x03 /**< Tells the font manager to draw this message in green */
#define SH3_MSG_GREEN       0x04 /**< Tells the font manager to draw this message in blue */

namespace sh3_system
{
    /**
     *  This class represents a printable message in SILENT HILL 3.
     *  The message itself is NOT handled here, rather, a list of messages and their
     *  appropriate IDs are loaded by the message manager (which will call @ref Draw() on our behalf.)
     */
    struct sh3_msg final
    {
    public:

        /**
         *  Load message and copy to our local string.
         *
         *  @param file Reference to the @ref sh3_arc_vfile that contains our message data.
         */
        void Load(sh3_arc_vfile& file);

        /**
         *  Draw the message to the screen. Unless directly forced by an attribute,
         *  this string will be drawn centered by ((scr_width - msgString.length) / 2) [I think this is the formula]
         */
        void Draw();

        std::string     msgString;      /**< The actual message, INCLUDING all of its attribyute bytes! */
        std::uint8_t    currentAttr;    /**< The current attribute we will print text with */
    };
}

#endif // MSG_HPP_INCLUDED
