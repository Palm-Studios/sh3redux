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
#include "SH3/system/glbuffer.hpp"
#include "SH3/system/glvertarray.hpp"
#include "SDL2/SDL.h"
#include "SH3/camera/camera.hpp"
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
    {{
        Target::ARRAY_BUFFER,
        Target::ARRAY_BUFFER
    }};
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

    sh3::camera::Camera cam(glm::vec3(0, 0, 0), angle<float>::FromDegrees(60.0f), static_cast<float>(640)/static_cast<float>(480), 0.001f, 100.0f);
    cam.SetPosition(glm::vec3(3, 4, 3));
    cam.LookAt(glm::vec3(0, 0, 0));
    cam.SetMode(sh3::camera::MODE::FIRST_PERSON);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLint mID = glGetUniformLocation(test.GetProgramID(), "mvp");
    glm::mat4 model{1.0f};

    bool forward = false;
    bool backward = false;

    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
                quit = true;

            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP)
                    forward = true;

                if(e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN)
                    backward = true;

                if(e.key.keysym.sym == SDLK_ESCAPE)
                    quit = true;
            }

            if(e.type == SDL_KEYUP)
            {
                if(e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP)
                    forward = false;

                if(e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN)
                    backward = false;
            }

            if(e.type == SDL_MOUSEMOTION)
            {
                int deltax = e.motion.xrel;
                int deltay = e.motion.yrel;

                cam.AddPitch(angle<float>::FromDegrees(static_cast<float>(-deltay) * 0.3f));
                cam.AddYaw(angle<float>::FromDegrees(static_cast<float>(deltax) * 0.3f));
            }
        }

        if(forward)
            cam.Translate(0.2f);
        if(backward)
            cam.Translate(-0.2f);

        glClear(GL_COLOR_BUFFER_BIT);
        test.Bind();
        glm::mat4 mvp = cam.GetProjectionMatrix() * model;
        glUniformMatrix4fv(mID, 1, GL_FALSE, &mvp[0][0]);
        triVao.Draw();
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_GL_SwapWindow(window.hwnd.get());
    }

    return static_cast<int>(exit_code::SUCCESS);
}
