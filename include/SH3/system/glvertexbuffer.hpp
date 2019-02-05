/** @file
 *
 *
 *  @copyright 2016-2019 Palm Studios
 *
 *  @date 3-2-2019
 *
 *  @author Jesse Buhagiar (quaker762)
 *
 */

#ifndef _GLVERTEXBUFFER_HPP_
#define _GLVERTEXBUFFER_HPP_

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <vector>

namespace sh3 { namespace gl {

struct CVertexBuffer final
{
public:
    /**
     * Buffer binding target enum
     *
     * Specifies the target to which the buffer object is bound for glBufferData.
     */
    enum BufferTarget : GLenum
    {
        ARRAY_BUFFER        = GL_ARRAY_BUFFER,                  /**< For Vertex Attributes */
        ATOMIC_COUNTER      = GL_ATOMIC_COUNTER_BUFFER,         /**< Atomic counter Storage */
        COPY_READ           = GL_COPY_READ_BUFFER,              /**< Buffer copy source */
        COPY_WRITE          = GL_COPY_WRITE_BUFFER,             /**< Buffer copy destination */
        ELEMENT_ARRAY       = GL_ELEMENT_ARRAY_BUFFER,          /**< Vertex array indices */
        PIXEL_PACK          = GL_PIXEL_PACK_BUFFER,             /**< Pixel Read target */
        PIXEL_UNPACK        = GL_PIXEL_UNPACK_BUFFER,           /**< Texture data source */
        QUERY               = GL_QUERY_BUFFER,                  /**< Query Result Buffer */
        SHADER_STORAGE      = GL_SHADER_STORAGE_BUFFER,         /**< Read-write storage for shaders */
        TEXTURE             = GL_TEXTURE_BUFFER,                /**< Texture data buffer */
        TRANSFORM_FEEDBACK  = GL_TRANSFORM_FEEDBACK_BUFFER,     /**< Transform feedback buffer */
        UNIFORM_BUFFER      = GL_UNIFORM_BUFFER                 /**< Uniform block storage */
    };

    /**
     * Buffer usage target enum
     *
     * "usage is a hint to the GL implementation as to how a buffer object's data store will be accessed.
     * This enables the GL implementation to make more intelligent decisions that may significantly impact
     * buffer object performance. It does not, however, constrain the actual usage of the data store.
     * usage can be broken down into two parts: first, the frequency of access (modification and usage),
     * and second, the nature of that access."
     */
    enum BufferUsage : GLenum
    {
        /**
         *  Draw usage
         *
         *  The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
         */
        STREAM_DRAW              = GL_STREAM_DRAW,       /**< The data store contents will be modified once and used at most a few times */
        STATIC_DRAW              = GL_STATIC_DRAW,       /**< The data store contents wil be modified once and used many times */
        DYNAMIC_DRAW             = GL_DYNAMIC_DRAW,      /**< The data store contents will be modified repeatedly and used many times */

        /**
         * Read usage
         *
         * The data store contents are modified by reading data from the GL, and used as the source for
         * GL drawing and image specification commands.
         */
        STREAM_READ             = GL_STREAM_READ,       /**< The data store contents will be modified once and used at most a few times */
        STATIC_READ             = GL_STATIC_READ,       /**< The data store contents wil be modified once and used many times */
        DYNAMIC_READ            = GL_DYNAMIC_READ,      /**< The data store contents will be modified repeatedly and used many times */

        /**
         * Copy usage
         *
         * The data store contents are modified by reading data from the GL, and used to return that data when queried by the application
         */
        STREAM_COPY             = GL_STREAM_COPY,       /**< The data store contents will be modified once and used at most a few times */
        STATIC_COPY             = GL_STATIC_COPY,       /**< The data store contents wil be modified once and used many times */
        DYNAMIC_COPY            = GL_DYNAMIC_COPY,      /**< The data store contents will be modified repeatedly and used many times */
    };

    static constexpr GLuint VBO_RESET = 0; /**< ""Reset"" VBO ID */

public:
    /**
     * Default constructor
     *
     * Will auto-generate a Vertex Buffer Object and store the result in @ref vboID
     */
    CVertexBuffer();

    /**
     */
    CVertexBuffer(const std::string& name);

    /**
     * Copy Constructor
     *
     * @warning A buffer cannot be explicitly copied via constructor.
     */
    CVertexBuffer(const CVertexBuffer& rhs) = delete;

    /**
     * Move constructor
     *
     * @warning A buffer cannot be moved.
     */
    CVertexBuffer(CVertexBuffer&& rhs) = delete;

    /**
     *
     */
    ~CVertexBuffer();

    /**
     * Fill this buffer (@ref vboID) with data.
     *
     * This effectively works as "GPU malloc()". It first generates a block of memory
     * on the GPU with size @ref size, gets the pointer for it and fills it with the data found in
     * @ref data.
     *
     * @param target    The target to which this buffer object is bound to
     * @param size      Size of the memory block we want to allocate in VRAM for this data
     * @param data      The data that we want to write to the block allocated by glBufferData()
     * @param usage     How this buffer is to be used. see @ref BufferUsage for more info.
     */
    void BufferData(BufferTarget target, GLsizeiptr size, const GLvoid* data, BufferUsage usage);

    /**
     * Fill this buffer (@ref vboID) with data.
     *
     * Identical to the above function, however, uses direct buffer storage. That is,
     * there is no need to bind this buffer to a target before a call to glBufferData
     *
     * @param size      Size of the memory block we want to allocate in VRAM for this data
     * @param data      The data that we want to write to the block allocated by glBufferData()
     * @param usage     How this buffer is to be used. see @ref BufferUsage for more info.
     */
    void BufferData(GLsizeiptr size, const void* data, BufferUsage usage);

    /**
     * Copy @c size bytes of data into the buffer allocated in VRAM, starting at address @c vbo_base + offset, where @c vbo_base is the addres
     * of the memory block in video memory.
     *
     * @param target    The target to which this buffer object is bound to
     * @param offset    Offset into the buffer to start writing
     * @param size      Size of the memory block we want to allocate in VRAM for this data
     * @param data      The data that we want to write to the block allocated by glBufferData()
     */
    void BufferSubData(BufferTarget target, GLintptr offset, GLsizeiptr size, const GLvoid* data);

    /**
     * Copy @c size bytes of data into the buffer allocated in VRAM, starting at address @c vbo_base + offset, where @c vbo_base is the addres
     * of the memory block in video memory.
     *
     * @param offset    Offset into the buffer to start writing
     * @param size      Size of the memory block we want to allocate in VRAM for this data
     * @param data      The data that we want to write to the block allocated by glBufferData()
     */
    void BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data);

    /**
     * Bind this buffer to a @ref BufferTarget target, making this VBO the active VBO.
     *
     * @param target The target we want to bind to.
     */
    inline void Bind(void) const noexcept;

    /**
     * Unbind this buffer
     */
    inline void Unbind(void) const noexcept;

    /**
     * Create a new Vertex Buffer Object
     */
    inline void Create(void) noexcept;

    /**
     * Destroy this buffer object
     */
    inline void Destroy(void) noexcept;

    /**
     * Returns this VBOs OpenGL generated ID
     */
    GLuint Get() const {return vboID;}

    /**
     *
     */
    BufferTarget GetTarget() const{return target;}

    const void* const Data(void) const{return reinterpret_cast<const GLubyte*>(data.data());}

    /**
     * Operator= overload
     *
     * Copies rhs into this Vertex Buffer
     *
     * @return Returns this CVertexBuffer
     */
    const CVertexBuffer& operator=(const CVertexBuffer& rhs);

    /**
     * Get the name of this VAO
     */
    const std::string& GetName() const {return name;}

private:
    GLuint                  vboID;          /**< ID of the Vertex Buffer Object given to us by OpenGL */
    std::string             name;           /**< Name of this vertex buffer */
    BufferTarget            target;         /**< The target that this buffer is to used for */

    std::vector<GLubyte>    data;           /**< Local data store of this buffer (so we can debug on the CPU without a need for a GL debugger [like CodeXL]) */
};


}}






#endif /* INCLUDE_SH3_SYSTEM_GLVERTEXBUFFER_HPP_ */
