/** @file
 *  Program entry point.
 *
 *  @copyright 2006  Palm Studios
 *
 *  @author Jesse Buhagiar
 */

#include "SH3/arc/mft.hpp"
#include "SH3/system/config.hpp"
#include "SH3/system/exit_code.hpp"
#include "SH3/system/log.hpp"
#include "SH3/system/window.hpp"
#include "SH3/system/shader.hpp"
#include "SH3/graphics/msbmp.hpp"
#include "SH3/graphics/texture.hpp"
#include "SH3/system/glvertexbuffer.hpp"
#include "SH3/system/glvertexarray.hpp"
#include "SH3/types/vertex.hpp"
#include <SDL.h>
#include <cstdio>

#include "SH3/engine/engine.hpp"

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

/**
 *  Entry point to the program.
 *
 *  @note Requires @c argc and @c argv to make it compatible with SDL2.
 *
 *  @param argc Number of arguments.
 *  @param argv Argument vector.
 *
 *  @returns An @ref exit_code specifying success or failure.
 */
int main(int argc, char** argv)
{
//    static_cast<void>(argc);
//    static_cast<void>(argv);
//
////    sh3::arc:: mft;
//
//    Log(LogLevel::INFO, "===SILENT HILL 3 REDUX===");
//    Log(LogLevel::INFO, "Copyright 2016-2017 Palm Studios\n");
//
//    sh3_window window(640, 480, "sh3redux");
//
//    bool quit = false;
//    SDL_Event e;
//
//    sh3::gl::CShader test("test");
//
//    using Triangle = sh3::gl::CVertexArray;
//
//    Triangle triVao;
//    sh3::gl::CVertexBuffer      vBuff;
//    sh3::gl::VertexAttribute    vAttr;
//    sh3::gl::CVertexBuffer      colBuff;
//    sh3::gl::VertexAttribute    colAttr;
//
//    // Setup vertex attribute
//    vAttr.idx = 0;
//    vAttr.normalize = false;
//    vAttr.offset = 0;
//    vAttr.size = 3;
//    vAttr.stride = 0;
//
//    vBuff.BufferData(sh3::gl::CVertexBuffer::BufferTarget::ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, sh3::gl::CVertexBuffer::BufferUsage::STATIC_DRAW);
//    triVao.BindAttribute(vAttr, vBuff, sh3::gl::VertexAttribute::AttributeType::FLOAT);
//
//    colAttr.idx = 1;
//    colAttr.normalize = false;
//    colAttr.offset = 0;
//    colAttr.size = 3;
//    colAttr.stride = 0;
//    colBuff.BufferData(sh3::gl::CVertexBuffer::BufferTarget::ARRAY_BUFFER, sizeof(glCol), glCol, sh3::gl::CVertexBuffer::BufferUsage::STATIC_DRAW);
//    triVao.BindAttribute(colAttr, colBuff, sh3::gl::VertexAttribute::AttributeType::FLOAT);
//
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    while(!quit)
//    {
//        while(SDL_PollEvent(&e) != 0)
//        {
//            if(e.type == SDL_QUIT)
//                quit = true;
//        }
//
//        glClear(GL_COLOR_BUFFER_BIT);
//        test.Bind();
//        triVao.Bind();
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        SDL_GL_SwapWindow(window.hwnd.get());
//    }

    sh3::engine::CEngine game;
    game.Init("");

    /**
     * Until we move a fair bit of stuff out of the construct (which is getting kinda dangerous at this point),
     * this method of init should NOT be used!
     */
    //game.Instance().Init("");
    return static_cast<int>(exit_code::SUCCESS);
}
