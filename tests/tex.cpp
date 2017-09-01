/** @file
 *  Texture test program.
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
#include <SDL.h>
#include <cstdio>

// Quad Co-Ordinates
static GLfloat vert_buffer[] =
{
    -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
};

static GLfloat uv_buffer[] =
{
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};

struct QuadAttributes final
{
private:
    using Target = sh3_gl::buffer_object::Target;

    QuadAttributes() = delete;

public:
    enum class Slot
    {
        VERTEX,
        UV,
        MAX
    };

    static constexpr sh3_gl::vao_target_array<Slot> Targets =
    {
        Target::ARRAY_BUFFER,
        Target::ARRAY_BUFFER
    };
};
constexpr sh3_gl::vao_target_array<QuadAttributes::Slot> QuadAttributes::Targets;

int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    Log(LogLevel::INFO, "===SILENT HILL 3 REDUX===");
    Log(LogLevel::INFO, "Copyright 2016-2017 Palm Studios\n");

    sh3_window window(1024, 768, "sh3redux | texture test");
    bool quit = false;
    SDL_Event ev;
    sh3_gl::program::load_error err;
    sh3_gl::program prog("image", err);
    sh3_arc mft;

    mft.Load(); // Load the Master File Table

    using Quad = sh3_gl::vao<QuadAttributes>;
    Quad quadVao;
    quadVao.Bind();

    sh3_gl::buffer_object& verts = quadVao[Quad::Slot::VERTEX];
    verts.BufferData(vert_buffer, sizeof(vert_buffer), GL_STATIC_DRAW);
    quadVao.SetDataLocation(Quad::Slot::VERTEX, Quad::DataType::FLOAT, 3, 0, 0);

    sh3_gl::buffer_object& uv = quadVao[Quad::Slot::UV];
    uv.BufferData(uv_buffer, sizeof(uv_buffer), GL_STATIC_DRAW);
    quadVao.SetDataLocation(Quad::Slot::UV, Quad::DataType::FLOAT, 2, 0, 0);

    quadVao.Unbind();

    sh3_graphics::sh3_texture tex(mft, "data/pic/sy/sys_title.tex");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "info", "You should now see a texture drawn on the screen.", nullptr);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    while(!quit)
    {
        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
                quit = true;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        prog.Bind();
        tex.Bind(GL_TEXTURE1);
        quadVao.Draw();
        SDL_GL_SwapWindow(window.hwnd.get());
    }

    return static_cast<int>(exit_code::SUCCESS);
}
