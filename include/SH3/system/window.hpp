/** @file
 *
 *  Window class and related functions
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note
 *
 *  \date 22-12-2016
 *
 *  \author Jesse Buhagiar
 */
#ifndef SH3_WINDOW_HPP_INCLUDED
#define SH3_WINDOW_HPP_INCLUDED

#include <memory>
#include <string>
#include <SDL2/SDL_video.h>
#include "SH3/stdtype.hpp"
#include "SH3/system/glcontext.hpp"

/**
 * Logical window. Gives a handle to OpenGL and SDL2.
 */
class sh3_window
{
private:

public:
    sh3_window(int width, int height, const std::string& title);

    std::unique_ptr<SDL_Window, sdl_destroyer> hwnd;

private:
    int width;
    int height;
    std::string title;

    sh3_glcontext context;
};


#endif // SH3_WINDOW_HPP_INCLUDED
