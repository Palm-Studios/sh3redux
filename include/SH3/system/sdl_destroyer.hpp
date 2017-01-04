/** @file
 *  A functor to destroy SDL objects
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 1-1-2017
 *
 *  @author Alexander Hirsch
 */
#ifndef SH3_SDL_DESTROYER_HPP_INCLUDED
#define SH3_SDL_DESTROYER_HPP_INCLUDED

#include <SDL2/SDL_video.h>

using flat_sdl_glcontext = std::remove_pointer<SDL_GLContext>::type;

/**
 *  Destroyer functor
 */
struct sdl_destroyer final
{
public:
    void operator()(SDL_Window* window) const {SDL_DestroyWindow(window);}
    void operator()(SDL_GLContext context) const {SDL_GL_DeleteContext(context);}
};

#endif // SH3_SDL_DESTROYER_HPP_INCLUDED
