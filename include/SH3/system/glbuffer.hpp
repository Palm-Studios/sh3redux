/** @file
 *
 *  A wrapper and more OO approach to OpenGL Vertex Buffer Objects. This is also helps prevent any
 *  memory problems as well as providing a whole lot of helper functions for our VBO.
 *
 *  @copyright 2017 Palm Studios
 *
 *  @date 8-6-2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef buffer_HPP_INCLUDED
#define buffer_HPP_INCLUDED

#include "GL/glew.h"
#include "GL/gl.h"

#include <string>
#include <cstddef>

namespace sh3_gl
{
    /**
     * An OpenGL Vertex Buffer Object (VBO). Provides a nice, easy, typesafe way
     * to create and manage OpenGL buffers.
     */
    struct buffer_object final
    {
    public:

        enum class Target
        {
            ARRAY_BUFFER    = GL_ARRAY_BUFFER,
            ELEMENT_BUFFER  = GL_ELEMENT_ARRAY_BUFFER,
            TEXTURE_BUFFER  = GL_TEXTURE_BUFFER
        };

        /**
         *  Constructor.
         *
         *  FIXME: We're currently <i>always</i> passing @ref Target::ARRAY_BUFFER to the buffer object,
         *  as we never actually pass anything to this constructor (as well as never passing a buffer_object to
         *  @ref mutablevao).
         */
        buffer_object(Target type, const std::string& buffName = ""): buffType(type), name(buffName){Create();};

        /**
         *  Destructor.
         */
        ~buffer_object(){/**Release(); FIXME: This causes a segmentation violation (why is this called in the game loop!?!?!)*/};

        /**
         *  Create our buffer and register it with OpenGL.
         */
        void Create();

        /**
         *  Free any data related to this buffer, instead of letting the destructor do it.
         *  We explicitly tell OpenGL to release the buffer.
         */
        void Release();

        /**
         *  Give some data to our buffer
         *
         *  @param data     void pointer to our data.
         *  @param num      Size (in bytes) of the data we are flushing.
         *  @param usage    The way in which want this data stored (i.e, can we rewrite sections of it?).
         */
        void BufferData(void* data, std::size_t num, GLenum usage);

        /**
         *  Redefines/updates a subset of data stored in this GLBuffer.
         *
         *  @param data void pointer to our data.
         *  @param num  Size (in bytes) of the data we are flushing.
         */
        void BufferSubData(void* data, std::size_t size);

        /**
         *  Bind our buffer for use.
         */
        void Bind() const;

        /**
         *  Unbind our buffer.
         */
        void Unbind();

        /**
         *  Get the ID of our buffer.
         *
         *  @return The ID of this buffer.
         */
        GLuint GetID() const {return id;}

        /**
         *  Return what type of buffer this is.
         *
         *  @return Type of this buffer (from @ref Target enum).
         */
        Target GetType() const {return buffType;}

        /**
         *  Get the number of elements contained within this buffer.
         *
         *  @return Number of elements in this GL Buffer.
         */
        std::uint32_t GetNumberElements() const;

        /**
         *  Return the size of this buffer.
         *
         *  @return Size of this GL Buffer in bytes.
         */
        std::size_t GetBufferSize() const;

    private:
        GLuint          id;         /**< The ID of our buffer given to us by OpenGL */
        Target          buffType;   /**< What type of buffer this is */
        std::size_t     size;       /**< The size of this buffer in bytes */
        std::string     name;       /**< The name of this buffer */
    };
}

#endif // buffer_HPP_INCLUDED
