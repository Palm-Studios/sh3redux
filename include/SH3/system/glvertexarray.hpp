/** @file
 *
 *  An abstraction of OpenGL Vertex Array Objects (VAO). A VAO is a container which contains one or more
 *  VBOs (Vertex Buffer Object), which is a GPU allocated block of memory (the highspeed GDDR4/5/6 stuff). The VAO keeps track
 *  of which buffers are bound. When a call to "Draw()" is made, using the current VAO to draw, the currently active
 *  @ref sh3::gl::CShader program works on this data at the specific bindings.
 *
 *  That's a bit of a mouthful, but it basically comes down to mapping which buffer to which index in the vertex stage of
 *  the rendering pipeline (the .vert shaders in /data/shaders). This way we can automatically work out which data is where in
 *  the GPU and perform the appropriate action in the shader (transformation, passing to fragment shader etc).
 *
 *  An example of this can actually be seen in the example shader image.vert in the following lines:
 *
 *      #version 420
 *      layout(location = 0) in vec3 vertexPosition;
 *      layout(location = 1) in vec2 vertexUV;
 *
 *  This tells the shader program to expect vertexPositions at VAO index "slot" 1, and the UV co-ordinates of the image at VAO index "slot" 2.
 *  These can be manually mapped to the shader during the link phase of the program with a vector of sh3::gl::ShaderAttribute structs.
 *
 *  @copyright 2016-2019 Palm Studios
 *
 *  @date 30-1-2019
 *
 *  @author Jesse Buhagiar (quaker762)
 *
 */
#ifndef _GLVERTEXARRAY_HPP_
#define _GLVERTEXARRAY_HPP_

#include <string>

#include "SH3/system/glvertexbuffer.hpp"

namespace sh3{ namespace gl{

/**
 *  Vertex Attribute structure
 */
struct VertexAttribute
{
    /**
     * Enum types for each component in the vertex buffer for attribute n
     */
    enum AttributeType : GLenum
    {
        BYTE                        = GL_BYTE,
        UNSIGNED_BYTE               = GL_UNSIGNED_BYTE,
        SHORT                       = GL_SHORT,
        UNSIGNED_SHORT              = GL_UNSIGNED_SHORT,
        INT                         = GL_INT,
        UNSIGNED_INT                = GL_UNSIGNED_INT,

        HALF_FLOAT                  = GL_HALF_FLOAT,
        FLOAT                       = GL_FLOAT,
        DOUBLE                      = GL_DOUBLE,
        FIXED_POINT                 = GL_FIXED,
        INT_2_10_10_10              = GL_INT_2_10_10_10_REV,
        UNSIGNED_INT_2_10_10_10     = GL_UNSIGNED_INT_2_10_10_10_REV,
        UNSIGNED_INT_10F_11F_11F    = GL_UNSIGNED_INT_10F_11F_11F_REV
    };

    GLuint      idx;        /**< Which index this attribute is mapped to via layout(location = ) in the vertex stage of the pipeline */
    GLint       size;       /**< Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4 */
    GLboolean   normalize;  /**< Should the data for this attribute be normalized by the GPU? */
    GLsizei     stride;     /**< Byte offset between attributes. That is, is we have memory layout of {vertex, normal, colour}, each of 8-bits, the stride would be 2 [bytes] */
    GLintptr    offset;     /**< Data offset for first element */
};

/**
 * OpenGL Vertex Array Object wrapper class. See this file's header for more information on what a VAO is.
 * This VAO is mostly just used to map bindings for buffers (say for a mesh). There are
 * other uses, however for our intents, that will be MORE than enough.
 *
 * This class doesn't really need to store any data per se; mostly just the VAO object ID
 * and name/other forms of metadata.
 *
 * @note This class <i>CANNOT</i> be inherited from!
 *
 */
struct CVertexArray final
{
public:
    static constexpr GLuint VAO_UNBIND = 0; /**< VAO Unbind value. 0 specifies that no VAO is currently bound */

public:
    /**
     * Default constructor, which will automatically create a VAO ID <i>without</i> specifying a name.
     * The default name is "VAO_DEFAULT"
     */
    CVertexArray();

    /**
     * Constructor specifying name of this VAO for lookup.
     *
     * @param name Name of this VAO (for use in a lookup etc)
     */
    CVertexArray(const std::string& name);

    /**
     * Copy Constructor
     */
    CVertexArray(const CVertexArray& rhs);

    /**
     * Move constructor
     */
    CVertexArray(CVertexArray&& rhs);

    /**
     * Destructor. Will automatically cleanup and free VAO;
     */
    ~CVertexArray();

    /**
     * Create a new Vertex Array Object and store in @ref vaoID
     */
    inline void Create(void) noexcept;

    /**
     *
     */
    inline void Destroy(void) noexcept;

    /**
     * Binds a vertex attribute to this VAO
     *
     * @param attrib    @ref VertexAttribute descriptor struct
     * @param vbo       The vertex buffer for which we want to map this attribute to
     * @param type          The type of data this is, e.g floating point, fixed point, integer etc
     */
    void BindAttribute(const VertexAttribute& attrib, const CVertexBuffer& vbo, VertexAttribute::AttributeType type);

    /**
     * Binds a vertex attribute to this VAO
     *
     * @param idx           Attribute index that this vbo will be bound to in the VAO
     * @param vbo           Buffer object
     * @param size          Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4
     * @param normalize     Should this data be normalized before passed to the shader (between 0 and 1)?
     * @param stride        Byte gap between attribute
     * @param offset        Offset of this data in the buffer
     * @param type          The type of data this is, e.g floating point, fixed point, integer etc
     */
    void BindAttribute(const GLuint idx, const CVertexBuffer& vbo, GLint size, GLboolean normalize, GLsizei stride, GLintptr offset, VertexAttribute::AttributeType type);

    /**
     * Bind this VAO as the current one in use by the OpenGL state machine.
     */
    void Bind(void) const noexcept{glBindVertexArray(vaoID);}

    /**
     * Unbind this VAO
     */
    void Unbind(void) const noexcept{glBindVertexArray(VAO_UNBIND);}

    /**
     * Get the VAO object ID allocated to us by OpenGL
     */
    const GLuint GetObjectID() const{return vaoID;}

    /**
     * Get the name of this VAO
     */
    const std::string& GetName() const {return name;}

private:
    GLuint      vaoID;      /**< ID Generated to us by OpenGL */
    std::string name;       /**< Name of this VAO */
};




}}






#endif
