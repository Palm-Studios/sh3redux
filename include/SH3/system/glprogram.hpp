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

#include "SH3/error.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace sh3_gl
{
    struct program final
    {
    public:
        enum class load_result
        {
            SUCCESS,
            COMPILATION_FAILED,
            LINK_FAILED,
            INVALID_VALUE,
            INVALID_OPERATION,
        };

        class load_error final : public error<load_result>
        {
        public:
            void set_error(load_result result);

            std::string message() const;

        private:
        };

        program(const std::string& name, load_error& err, const std::vector<std::string>& attribs = {}) : programName(name){Load(name, err, attribs);};
        ~program(){Unbind(); glDeleteProgram(programID);}

        void Load(const std::string& name);
        /**
         *  Load a shader from a file on disk, compile and then link it, binding any attributes in the process.
         *
         *  @param shader Name of the shader we want to load (path is hard-coded to /data/shaders). Assumes *.vert and *.frag have the same name.
         *  @param attribs Vector of attributes we bind before we link the program.
         *  @param err Error set by this operation.
         *
         *  @return @ref load_error The error set by this operation (specified in the @ref load_result enum)
         */
        void Load(const std::string& shader, load_error& err, const std::vector<std::string>  &attribs = {});

        /**
         *  Bind this shader program for use.
         */
        void Bind();

        /**
         *  Unbind this program and stop using it.
         */
        void Unbind();

        /**
         *  Get the location of a uniform in our compiled GL Program.
         *
         *  @param name - The name of the uniform we want the location of.
         *
         *  @return GLint - Location of this uniform, where -1
         */
         GLint GetUniformLoc(const std::string& name);

        /**
         *  Set the value of a 2 value matrix in this program
         *
         *  @param name - Name of the uniform we want to load the data into.
         *  @param mat - The matrix we want to send to the program.
         */
         void SetUniformMat(const std::string& name, const glm::mat2& mat);

        /**
         *  Set the value of a 3 value matrix in this program
         *
         *  @param name - Name of the uniform we want to load the data into.
         *  @param mat - The matrix we want to send to the program.
         */
         void SetUniformMat(const std::string& name, const glm::mat3& mat);

        /**
         *  Set the value of a 4 value matrix in this program
         *
         *  @param name - Name of the uniform we want to load the data into.
         *  @param mat - The matrix we want to send to the program.
         */
         void SetUniformMat(const std::string& name, const glm::mat4& mat);

        /**
         *  Set the value of a 2D vector in this program
         *
         *  @param name - Name of the uniform we want to load the data into.
         *  @param vec - The vector we want to send to the program.
         */
         void SetUniformVec(const std::string& name, const glm::vec2& vec);

        /**
         *  Set the value of a 3D vector in this program
         *
         *  @param name - Name of the uniform we want to load the data into.
         *  @param vec - The vector we want to send to the program.
         */
         void SetUniformVec(const std::string& name, const glm::vec3& vec);

        /**
         *  Set the value of a 3D vector in this program
         *
         *  @param name - Name of the uniform we want to load the data into.
         *  @param vec - The vector we want to send to the program.
         */
         void SetUniformVec(const std::string& name, const glm::vec4& vec);

        /**
         *  Set the value of an integer in this program
         *
         *  @param name - Name of the uniform we want to load the data into.
         *  @param val - The value of the integer.
         */
         void SetUniformInt(const std::string& name, const int val);

        /**
         *  Set the value of a float in this program
         *
         *  @param name - Name of the uniform we want to load the data into.
         *  @param val - The value of the float.
         */
         void SetUniformFloat(const std::string& name, const float val);

        /**
         *  Get the ID of our compiled GL Program. This is used mainly for glUniformXYZ( ) functions
         *  for interfacing between sh3redux C++ and GLSL.
         *
         *  @return GLuint - ID of this program
         */
         GLuint GetProgramID() const;

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
