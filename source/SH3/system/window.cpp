/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_window.cpp

Abstract:
        Implementation of sh3_window.hpp

Author:
        Jesse Buhagiar

Environment:

Notes:

Revision History:
        22-12-2016: File Created                                    [jbuhagiar]


--*/
#include "SH3/system/glcontext.hpp"
#include "SH3/system/window.hpp"
#include "SH3/system/log.hpp"

using namespace sh3::system;

CWindow::CWindow(int width, int height, const std::string& title)
    : fullscreen(false), title(title), hwnd(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL)), context(*this)
{
    // Populate display mode list
    SDL_DisplayMode curr;
    for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
    {
        int ret = SDL_GetCurrentDisplayMode(i, &curr);
        if(ret != 0)
        {
            std::string err = "Could not get display mode for video display index ";
            err += i;
            err +=  "! ";
            err += SDL_GetError();

            die(err.c_str());
        }

        displayModes.push_back(&curr);   // Add the current display mode to the list of display modes
    }
}

void CWindow::Create(int width, int height, const std::string& _title)
{
    // TODO: Use cvar/config file to set this instead of us doing it statically
    fullscreen = false;
    title = _title;

    hwnd.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL));
    context = CRenderContext(*this);

    // Populate display mode list
    SDL_DisplayMode curr;
    for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
    {
        int ret = SDL_GetCurrentDisplayMode(i, &curr);
        if(ret != 0)
        {
            std::string err = "Could not get display mode for video display index ";
            err += i;
            err +=  "! ";
            err += SDL_GetError();

            die(err.c_str());
        }

        displayModes.push_back(&curr);   // Add the current display mode to the list of display modes
    }
}

void CWindow::SetSize(int width, int height)
{
    Destroy();

    hwnd.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL));
    context = CRenderContext(*this);

//    // Do a check to see if we're in fullscreen. If we are
//    // we'll use one of the video modes we've pre-scanned for!
//    if(!fullscreen)
//    {
//        SDL_SetWindowSize(hwnd.get(), width, height);
//    }
//    else
//    {
//        // TODO: Search the list of display modes for the correct one!
//        for(SDL_DisplayMode* mode : displayModes)
//        {
//            if(mode->w == width && mode->h == height)
//            {
//                SDL_SetWindowDisplayMode(hwnd.get(), mode);
//                break;
//            }
//        }
//    }
//
//    context = CRenderContext(*this); // Refresh the render context
}

void CWindow::Destroy(void)
{
    hwnd.reset();
}

const SDL_Window* CWindow::GetHandle(void) const noexcept
{
    return hwnd.get();
}
