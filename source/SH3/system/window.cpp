/** @file
 *
 *  Implementation of sh3_window.hpp
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note
 *
 *  \date 22-12-2016
 *
 *  \author Jesse Buhagiar
 */
#include "SH3/system/glcontext.hpp"
#include "SH3/system/window.hpp"

sh3_window::sh3_window(int width, int height, const std::string& title)
    : hwnd(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL))
    , context(*this)
{
    context.PrintInfo();
}
