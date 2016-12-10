/*++

Copyright (c) 2016  Palm Studios

Module Name:
        CWindow.hpp

Abstract:
        Handles and data to provide better access to an SDL_Window.
        Basically a wrapper as SDL is a C library.

Author:
        Quaker762

Environment:

Notes:

Revision History:


--*/
#ifndef CWINDOW_HPP_INCLUDED
#define CWINDOW_HPP_INCLUDED

#include <SDL2/SDL.h>
#include <string>

void glPrintString(const char* str, int x, int y);

class CWindow
{
public:
    CWindow(int width, int height);
    ~CWindow();

    int getWidth();
    int getHeight();

    void winSwap();

//////////////OPENGL RELATED FUNCTIONS//////////////

    std::string  glGetVersion();

private:
    int             width; // Screen width
    int             height; // Screen height
    bool            fullscreen; // Is this Window fullscreen?

    SDL_Window*     hwnd; // Handle to physical window
    SDL_GLContext   glContext;

};




#endif // CWINDOW_HPP_INCLUDED
