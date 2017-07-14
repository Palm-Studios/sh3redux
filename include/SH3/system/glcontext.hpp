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

class sh3_window;

namespace sh3_gl
{
    class context
    {
    public:
        explicit context(sh3_window& hwnd);

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

       /**
        *   Calculate our projection matrix (how we project vertices on the Z plane on the 2D screen).
        *   Note that OpenGL has +Z coming TOWARDS the user, which is slightly unintuitive, as it means
        *   each vertex must have its' Z value inverted.
        *
        *   @param fov - The Field Of View (FOV) of our Viewing Frustum. Defined in @ref sh3_graphics::camera.
        *   @param near - The near plane of our Viewing Frustum, i.e, when objects will disappear.
        *   @param far - The far plane of our Viewing Frustum, i.e, when far objects will disappear (SH fog etc).
        *
        *   @return glm::mat4 - 4 float matrix we send to our GLSL program.
        */
        glm::mat4 GetProjectionMatrix(float fov, int width, int height, float near, float far);

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
}

#endif // SH3_GLCONTEXT_HPP_INCLUDED
