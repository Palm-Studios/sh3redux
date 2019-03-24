/** @file
 *  Defines a logical OpenGL Context
 *
 *  @copyright 2016-2017 Palm Studios
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
#include <glm/glm.hpp>

#include "SH3/system/sdl_destroyer.hpp"

namespace sh3 { namespace system {

class CWindow;  /**< Forward declaration to prevent circular inclusion */

/**
 * Our OpenGL render context.
 */
class CRenderContext
{
public:

    CRenderContext(){}

    /**
     * Constructor
     *
     * @param hwnd Physical SDL Window Reference
     */
    explicit CRenderContext(CWindow& hwnd);

    /**
     * Create a new OpenGL Render Context
     *
     * @param hwnd Physical SDL Window Reference
     */
    void Create(CWindow& hwnd);

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
    *  Get the name of the renderer (Graphics Card that created this context).
    *
    *  @return OpenGL Renderer name (Graphics Card name).
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
    *  @param name - Name of symbolic constant we want to get from the driver.
    *
    *  @return char* from @c glGetString.
    */
    static const char* GlGetString(GLenum name);

private:
    std::unique_ptr<flat_sdl_glcontext, sdl_destroyer> glContext;   /**< A pointer to our actual OpenGL Context */
    std::vector<const char*> extensions;                            /**< List of all extensions supported by this OpenGL Driver */
};

}}

#endif // SH3_GLCONTEXT_HPP_INCLUDED
