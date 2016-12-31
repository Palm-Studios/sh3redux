/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_glcontext.hpp

Abstract:
        Defines a logical OpenGL Context

Author:
        Quaker762

Environment:

Notes:
    https://www.khronos.org/opengl/wiki/GLAPI/glGet

Revision History:
        22-12-2016: File Created                                    [Quaker762]

--*/
#ifndef SH3_GLCONTEXT_HPP_INCLUDED
#define SH3_GLCONTEXT_HPP_INCLUDED

#include <memory>
#include <SH3/system/sh3_sdl_destroyer.hpp>

class sh3_window;

class sh3_glcontext
{
public:
    sh3_glcontext(sh3_window* hwnd);

    char* GetVendor();
    char* GetVersion();
    char* GetRenderer();

    void GetExtensions();
    void PrintInfo();

private:
    std::unique_ptr<flat_sdl_glcontext, sdl_destroyer> glContext;
    char**        extensions;

};


#endif // SH3_GLCONTEXT_HPP_INCLUDED
