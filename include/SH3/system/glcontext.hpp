/** @file
 *
 *  Defines a logical OpenGL Context
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note   https://www.khronos.org/opengl/wiki/GLAPI/glGet
 *
 *  \date 22-12-2016
 *
 *  \author Jesse Buhagiar
 */
#ifndef SH3_GLCONTEXT_HPP_INCLUDED
#define SH3_GLCONTEXT_HPP_INCLUDED

#include <memory>
#include <vector>
#include <GL/glew.h>
#include "SH3/system/sdl_destroyer.hpp"

class sh3_window;

/**
 * Class that allows us to interact more easily with OpenGL, as well as defining some helper functions
 */
class sh3_glcontext
{
public:
    sh3_glcontext(sh3_window& hwnd);

    const char* GetVendor() const;
    const char* GetVersion() const;
    const char* GetRenderer() const;

    void GetExtensions();
    void PrintInfo() const;

private:
    static const char* GlGetString(GLenum name);

private:
    std::unique_ptr<flat_sdl_glcontext, sdl_destroyer> glContext;
    std::vector<const char*> extensions;

};


#endif // SH3_GLCONTEXT_HPP_INCLUDED
