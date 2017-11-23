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
#include "SH3/system/glprogram.hpp"
#include "SH3/graphics/msbmp.hpp"
#include "SH3/graphics/texture.hpp"
#include "SH3/system/glbuffer.hpp"
#include "SH3/system/glvertarray.hpp"
#include "SH3/types/vertex.hpp"
#include <SDL.h>
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

/**
 *  VAO Attributes for a colorful triangle.
 */
struct TriangleAttributes final
{
private:
    using Target = sh3_gl::buffer_object::Target;

    TriangleAttributes() = delete;

public:
    enum class Slot
    {
        VERTEX,
        COLOR,
        MAX
    };

    static constexpr sh3_gl::vao_target_array<Slot> Targets =
    { {
        Target::ARRAY_BUFFER,
        Target::ARRAY_BUFFER
    } };
};
constexpr sh3_gl::vao_target_array<TriangleAttributes::Slot> TriangleAttributes::Targets;

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
    static_cast<void>(argc);
    static_cast<void>(argv);

//    sh3::arc:: mft;

    Log(LogLevel::INFO, "===SILENT HILL 3 REDUX===");
    Log(LogLevel::INFO, "Copyright 2016-2017 Palm Studios\n");

    sh3_window window(640, 480, "sh3redux");

    bool quit = false;
    SDL_Event e;

    sh3_gl::program::load_error err;
    sh3_gl::program test("test", err);

    using Triangle = sh3_gl::vao<TriangleAttributes>;

    Triangle triVao;

    triVao.Bind();

    sh3_gl::buffer_object& verts = triVao[Triangle::Slot::VERTEX];
    verts.BufferData(g_vertex_buffer_data, sizeof(g_vertex_buffer_data), GL_STATIC_DRAW);
    triVao.SetDataLocation(Triangle::Slot::VERTEX, Triangle::DataType::FLOAT, 3, 0, 0);

    sh3_gl::buffer_object& col = triVao[Triangle::Slot::COLOR];
    col.BufferData(glCol, sizeof(glCol), GL_STATIC_DRAW);
    triVao.SetDataLocation(Triangle::Slot::COLOR, Triangle::DataType::FLOAT, 3, 0, 0);

    triVao.Unbind();

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
        triVao.Draw();
        SDL_GL_SwapWindow(window.hwnd.get());
    }

    return static_cast<int>(exit_code::SUCCESS);
}
