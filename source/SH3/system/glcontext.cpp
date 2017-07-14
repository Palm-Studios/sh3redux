/** @file
 *
 *  Implementation of glcontext.hpp
 *
 *  @copyright 2016-2017 Palm Studios
 *
 *  @date 22-12-2016
 *
 *
 *  @author Jesse Buhagiar
 */
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <limits>

#include "SH3/system/glcontext.hpp"
#include "SH3/system/log.hpp"
#include "SH3/system/window.hpp"
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace sh3_gl;

context::context(sh3_window& hwnd)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    glContext.reset(SDL_GL_CreateContext(hwnd.hwnd.get()));

    if(glewInit() != GLEW_OK) // Initialise GLEW!
    {
        die("sh3_glcontext::sh3_glcontext( ): GLEW Init failed! (Does your Graphics Driver support OpenGL 3.3?)");
    }

    // Set the colour size for OpenGL!
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

const char* context::GetVendor() const
{
    return GlGetString(GL_VENDOR);
}

const char* context::GetVersion() const
{
    return GlGetString(GL_VERSION);
}

const char* context::GetRenderer() const
{
    return GlGetString(GL_RENDERER);
}

void context::GetExtensions()
{
    GLint numExts;
    GLint i;

    glGetIntegerv(GL_NUM_EXTENSIONS, &numExts); // Get the number of extensions the system supports
    static_assert(std::numeric_limits<std::size_t>::max() >= std::numeric_limits<GLint>::max(), "std::size_t must be able to represent all positive GLint");
    assert(numExts >= 0);
    extensions.reserve(static_cast<std::size_t>(numExts));

    // Iterate over each extension the graphics card supports and store it
    for(i = 0; i < numExts; i++)
    {
        extensions.push_back(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
    }
}

void context::PrintInfo() const
{
    Log(LogLevel::INFO, "GL_VENDOR:\t %s", GetVendor());
    Log(LogLevel::INFO, "GL_VERSION:\t %s", GetVersion());
    Log(LogLevel::INFO, "GL_RENDERER:\t %s", GetRenderer());

    std::printf("GL_VENDOR:\t %s\n", GetVendor());
    std::printf("GL_VERSION:\t %s\n", GetVersion());
    std::printf("GL_RENDERER:\t %s\n", GetRenderer());
}

const char* context::GlGetString(GLenum name)
{
    return reinterpret_cast<const char*>(glGetString(name));
}

glm::mat4 context::GetProjectionMatrix(float fov, int width, int height, float near, float far)
{
    return glm::perspective(fov, static_cast<float>(width) / static_cast<float>(height), near, far);
}
