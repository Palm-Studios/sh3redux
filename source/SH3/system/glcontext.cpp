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
#include "SH3/system/assert.hpp"
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <glm/gtc/matrix_transform.hpp>

/**
 *  OpenGL error callback function. 
 *
 *  Used in conjunction with glDebugMessageCallback.
 *
 *  @arg source     The Source of this error.
 *  @arg type       The type of error this is.
 *  @arg id         Error ID.
 *  @arg severity   How severe this error is. If it's fatal, terminate the program with @ref die().
 *  @arg length     Length of the message log.
 *  @arg message    Error message passed to us from OpenGL.
 *  @arg data       User Param data.
 */
static void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* data)
{   
    LogLevel    level;
    std::string errMsg;
    
    static_cast<void>(id);
    static_cast<void>(length);
    
    ASSERT(!data);
    errMsg = "";

    // Get the source of the error message
    switch(source)
    {
    case GL_DEBUG_SOURCE_API: // Error caused by the OpenGL API
        errMsg += "API ";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        errMsg += "WINDOW SYSTEM ";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        errMsg += "SHADER COMPILER ";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        errMsg += "APPLICATION ";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        errMsg += "OTHER ";
        break;
    default:
        errMsg += "UNKNOWN ";
        break;
    }

    // Get the type of error this is
    switch(type)
    {
    case GL_DEBUG_TYPE_ERROR:
        errMsg += "(ERROR): ";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        errMsg += "(DEPRECATED): ";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        errMsg += "(UNDEFINED BEHAVIOR): ";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        errMsg += "PORTABILITY! ";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        errMsg += "(PERFORMANCE): ";
        break;
    case GL_DEBUG_TYPE_OTHER:
        errMsg += "(OTHER): ";
        break;
    case GL_DEBUG_TYPE_MARKER:
        errMsg += "(MARKER): ";
        break;
    default:
        errMsg += "(UNKNOWN): ";
        break;
    }

    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            level = LogLevel::FATAL;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            level = LogLevel::ERROR;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            level = LogLevel::WARN;
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            level = LogLevel::INFO;
            break;
        default:
            level = LogLevel::INFO;
            break;
    }

    errMsg += message;
    Log(level, errMsg.c_str());
}

using namespace sh3_gl;

context::context(sh3_window& hwnd)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Put the context into 'forwrad compatible' mode, meaning no deprecated functionality will be allowed AT ALL!

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

    if(GLEW_KHR_debug)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else
    {
        Log(LogLevel::WARN, "GL_KHR_debug not available on this OpenGL context!");
    }
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

    glGetIntegerv(GL_NUM_EXTENSIONS, &numExts); // Get the number of extensions the system supports
    assert(numExts >= 0);
    static_assert(std::numeric_limits<GLuint>::max() >= std::numeric_limits<decltype(numExts)>::max(), "GLuint must be able to represent all positive values of numExts");
    GLuint uNumExts = static_cast<GLuint>(numExts);
    extensions.reserve(uNumExts);

    // Iterate over each extension the graphics card supports and store it
    for(GLuint i = 0; i < uNumExts; i++)
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
