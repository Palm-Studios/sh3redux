/** @file
 *  Defines a logical OpenGL Context
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 22-12-2016
 *
 *  @note https://www.khronos.org/opengl/wiki/GLAPI/glGet
 *
 *  @author Jesse Buhagiar
 */
#ifndef SH3_GLCONTEXT_HPP_INCLUDED
#define SH3_GLCONTEXT_HPP_INCLUDED

#include <memory>
#include <vector>
#include <GL/glew.h>
#include "SH3/system/sdl_destroyer.hpp"

class sh3_window;

class sh3_glcontext
{
public:
    explicit sh3_glcontext(sh3_window& hwnd);

    /**
     *  Return the vendor of OpenGL Driver in use.
     */
    const char* GetVendor() const;

    /**
     *  Get the version string of the OpenGL context currently in use.
     *
     *  @return OpenGL Version String.
     */
    const char* GetVersion() const;

    /**
     *  Get the name of the renderer.
     *
     *  @return OpenGL Renderer name (Graphics Card name)
     */
    const char* GetRenderer() const;

    /**
     *  Get a list of all extensions supported by the user's graphics card.
     */
    void GetExtensions();

    /**
     *  Print out information about the current GL Context.
     */
    void PrintInfo() const;

private:

    /**
     *  Get a string from the current GL Context.
     *
     *  @return String from @c glGetString
     */
    static const char* GlGetString(GLenum name);

private:
    std::unique_ptr<flat_sdl_glcontext, sdl_destroyer> glContext;
    std::vector<const char*> extensions;

};

#endif // SH3_GLCONTEXT_HPP_INCLUDED
