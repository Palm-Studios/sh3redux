/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_window.hpp

Abstract:
        Window class and related functions

Author:
        Jesse Buhagiar

Environment:

Notes:

Revision History:
        22-12-2016: File Created                                    [jbuhagiar]

--*/
#ifndef SH3_WINDOW_HPP_INCLUDED
#define SH3_WINDOW_HPP_INCLUDED

#include <memory>
#include <string>
#include <SDL2/SDL_video.h>
#include <SH3/stdtype.hpp>
#include <SH3/system/sh3_glcontext.hpp>

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
