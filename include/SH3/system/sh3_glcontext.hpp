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

#include <SDL2/SDL.h>

class sh3_window;

class sh3_glcontext
{
public:
    sh3_glcontext(sh3_window* hwnd);
    ~sh3_glcontext(){if(glContext!=NULL)SDL_GL_DeleteContext(glContext);}

    char* GetVendor();
    char* GetVersion();
    char* GetRenderer();

    void GetExtensions();
    void PrintInfo();

private:
        SDL_GLContext   glContext;
        char**          extensions;

};


#endif // SH3_GLCONTEXT_HPP_INCLUDED
