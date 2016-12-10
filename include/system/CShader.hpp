/*++

Copyright (c) 2016  Palm Studios

Module Name:
        CShader.hpp

Abstract:
        Defines a logical shader program that can be bound and updated in a scene.

Author:
        Quaker762

Environment:

Notes:

Revision History:
        5-12-2016: Initial Revision

--*/
#ifndef CSHADER_HPP_INCLUDED
#define CSHADER_HPP_INCLUDED

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>

class CShader
{
public:
    CShader(const char* vsource, const char* fsource, std::string shader_name);
    virtual ~CShader() = 0;

    GLuint  getProgram();
    GLint   getShaderVariable(const char* name);

protected:

private:
    bool compile(const char* vsource, const char* fsource);

    GLuint program;
    GLuint vertShader;
    GLuint fragShader;

    std::string shader_name;
};



#endif // CSHADER_HPP_INCLUDED
