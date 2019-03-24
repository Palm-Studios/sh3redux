/** @file
 *  Texture test program.
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

int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    Log(LogLevel::INFO, "===SILENT HILL 3 REDUX===");
    Log(LogLevel::INFO, "Copyright 2016-2017 Palm Studios\n");

    sh3::system::CWindow window(1024, 768, "sh3redux | texture test");
    bool quit = false;
    SDL_Event ev;
    sh3::gl::CShader prog("image");
    sh3::arc::mft mft;

    using Quad = sh3::gl::CVertexArray;

    Quad quadVao2;
    sh3::gl::CVertexBuffer      vertBuff;
    sh3::gl::VertexAttribute    vertAttribs;
    sh3::gl::CVertexBuffer      uvBuff;
    sh3::gl::VertexAttribute    uvAttribs;

    vertBuff.BufferData(sh3::gl::CVertexBuffer::BufferTarget::ARRAY_BUFFER, sizeof(vert_buffer), vert_buffer, sh3::gl::CVertexBuffer::BufferUsage::STATIC_DRAW);
    vertAttribs.idx = 0;
    vertAttribs.normalize = false;
    vertAttribs.offset = 0;
    vertAttribs.size = 3;
    vertAttribs.stride = 0;
    quadVao2.BindAttribute(vertAttribs, vertBuff, sh3::gl::VertexAttribute::AttributeType::FLOAT);

    uvBuff.BufferData(sh3::gl::CVertexBuffer::BufferTarget::ARRAY_BUFFER, sizeof(uv_buffer), uv_buffer, sh3::gl::CVertexBuffer::BufferUsage::STATIC_DRAW);
    uvAttribs.idx = 1;
    uvAttribs.normalize = false;
    uvAttribs.offset = 0;
    uvAttribs.size = 2;
    uvAttribs.stride = 0;
    quadVao2.BindAttribute(uvAttribs, uvBuff, sh3::gl::VertexAttribute::AttributeType::FLOAT);

    sh3::graphics::CTexture tex("data/pic/kcet.bmp");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "info", "You should now see a texture drawn on the screen.", nullptr);

    prog.Bind();

    while(!quit)
    {
        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
                quit = true;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        tex.Bind(GL_TEXTURE0);
        quadVao2.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        SDL_GL_SwapWindow(const_cast<SDL_Window*>(window.GetHandle()));
    }

    return static_cast<int>(exit_code::SUCCESS);
}
