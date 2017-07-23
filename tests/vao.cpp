/** @file
 *  Program entry point.
 *
 *  @copyright 2006  Palm Studios
 *
 *  @author Jesse Buhagiar
 */

#include "SH3/arc/types.hpp"
#include "SH3/system/config.hpp"
#include "SH3/system/exit_code.hpp"
#include "SH3/system/log.hpp"
#include "SH3/system/window.hpp"
#include "SH3/system/glprogram.hpp"
#include "SH3/graphics/msbmp.hpp"
#include "SH3/graphics/texture.hpp"
#include "SH3/system/glbuffer.hpp"
#include "SH3/system/glvertarray.hpp"
#include "SH3/types/vertex.hpp"
#include "SDL2/SDL.h"
#include <cstdio>

static GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

static GLfloat glCol[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

enum MyAttributes
{
    VERTEX_SLOT,
    COLOR_SLOT,
    MAX
};

int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    Log(LogLevel::INFO, "===SILENT HILL 3 REDUX===");
    Log(LogLevel::INFO, "Copyright 2016-2017 Palm Studios\n");

    sh3_window window(640, 480, "sh3redux"); // Window initialization

    bool quit = false;
    SDL_Event e;

    sh3_gl::program::load_error err;
    sh3_gl::program test("test", err);
    sh3_gl::vao<MyAttributes> myVao;

    myVao.Bind(); // Bind our VAO
	
	// Create the vertex buffer object
    sh3_gl::buffer_object& verts = myVao[MyAttributes::VERTEX_SLOT];
    verts.BufferData(g_vertex_buffer_data, sizeof(g_vertex_buffer_data), GL_STATIC_DRAW);
    myVao.SetDataLocation(MyAttributes::VERTEX_SLOT, sh3_gl::vao<MyAttributes>::DataType::FLOAT, 3, 0, 0);

	// Create the color buffer object
    sh3_gl::buffer_object& col = myVao[MyAttributes::COLOR_SLOT];
    col.BufferData(glCol, sizeof(glCol), GL_STATIC_DRAW);
    myVao.SetDataLocation(MyAttributes::COLOR_SLOT, sh3_gl::vao<MyAttributes>::DataType::FLOAT, 3, 0, 0);

    myVao.Unbind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
                quit = true;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        test.Bind();
        myVao.Bind();
        myVao.Draw();
        SDL_GL_SwapWindow(window.hwnd.get());
    }

    return static_cast<int>(exit_code::SUCCESS);
}
