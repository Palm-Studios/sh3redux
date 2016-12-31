/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_sdl_destroyer.hpp

Abstract:
        a functor to destroy SDL objects

Author:
        Alexander Hirsch

Environment:

Notes:

Revision History:

--*/
#ifndef SH3_SDL_DESTROYER_HPP_INCLUDED
#define SH3_SDL_DESTROYER_HPP_INCLUDED

#include <SDL2/SDL_video.h>

using flat_sdl_glcontext = std::remove_pointer<SDL_GLContext>::type;

struct sdl_destroyer final
{
public:
    void operator()(SDL_Window* window) const {SDL_DestroyWindow(window);}
    void operator()(SDL_GLContext context) const {SDL_GL_DeleteContext(context);}
};

#endif // SH3_SDL_DESTROYER_HPP_INCLUDED
