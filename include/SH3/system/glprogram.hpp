/** @file
 *
 *  Class to load and bind an OpenGL program.
 *  Due to the nature of the engine, it is assumed at any given time, a program will
 *  only consist of a vertex and a fragment shader.
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 26-1-2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef GLPROGRAM_HPP_INCLUDED
#define GLPROGRAM_HPP_INCLUDED

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>

namespace sh3_graphics
{

    struct sh3_glprogram final
    {
    public:

        sh3_glprogram(const std::string& name) : programName(name){Load(name);};
        ~sh3_glprogram(){Unbind(); glDeleteProgram(programID);}

        /**
         *  Load a shader from a file on disk, compile and then link it.
         *
         *  @param shader Name of the shader we want to load (path is hard-coded to /data/shaders). Assumes *.vert and *.frag have the same name.
         *
         *  @return GLint true or false depending on whether this shader was loaded correctly.
         */
        GLint Load(const std::string& shader);

        /**
         *  Bind this shader program for use.
         */
        void Bind();

        /**
         *  Unbind this program and stop using it.
         */
        void Unbind();

    private:
        GLuint programID;           /**< The ID of this program. */
        GLuint vertShader;          /**< The ID of the vertex shader. */
        GLuint fragShader;          /**< The ID of the fragment shader. */

        std::string programName;    /**< Name of this program */

        /**
         *  Compile a shader, given it's type.
         *
         *  @param type The type of shader we want to compile (either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
         *
         *  @return GLuint ID of this shader program.
         */
        GLuint Compile(GLenum type);
    };
}

#endif // GLPROGRAM_HPP_INCLUDED
