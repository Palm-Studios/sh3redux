/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_window.hpp

Abstract:
        Window class and related functions

Author:
        Quaker762

Environment:

Notes:

Revision History:
        22-12-2016: File Created                                    [Quaker762]

--*/
#ifndef SH3_WINDOW_HPP_INCLUDED
#define SH3_WINDOW_HPP_INCLUDED

#include <string>
#include <SDL2/SDL.h>
#include <SH3/stdtype.hpp>

class sh3_glcontext;

class sh3_window
{
public:
    sh3_window(int width, int height, std::string title);
    ~sh3_window(){safedelete(context);}

    SDL_Window*     hwnd;

private:
    int width;
    int height;
    std::string title;

    sh3_glcontext*  context;
};


#endif // SH3_WINDOW_HPP_INCLUDED
