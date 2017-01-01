/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_glcontext.hpp

Abstract:
        Defines a logical OpenGL Context

Author:
        Jesse Buhagiar

Environment:

Notes:
    https://www.khronos.org/opengl/wiki/GLAPI/glGet

Revision History:
        22-12-2016: File Created                                    [jbuhagiar]

--*/
#ifndef SH3_GLCONTEXT_HPP_INCLUDED
#define SH3_GLCONTEXT_HPP_INCLUDED

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <SH3/system/sh3_sdl_destroyer.hpp>

class sh3_window;

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
