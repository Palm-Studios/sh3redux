/** @file
 *
 *  An abstraction of OpenGL Vertex Array Objects (VAO). Each mesh has its' own VAO, making it a lot simpler
 *  to reuse meshes (for instance, just after Heather leaves the bathroom in the corridor with 3-4 Double Heads).
 *
 *  Also gives us a nice abstract and typesafe way to deal with VAOs.
 *
 *  @copyright 2017 Palm Studios
 *
 *  @date 8-6-2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef GLVERTARRAY_HPP_INCLUDED
#define GLVERTARRAY_HPP_INCLUDED

#include <vector>
#include <array>

#include "GL/glew.h"
#include "GL/gl.h"

#include "SH3/system/assert.hpp"
#include "SH3/system/glbuffer.hpp"

namespace sh3_gl{

    /**
     *  Contains actual functionality of our VAO (@ref Bind, @ref Create etc).
     *
     *  @note Should <i>not</i> be directly instantiated, use @ref vao instead.
     */
    struct finalvao
    {
    protected:

        /**
         *  Non indexed constructor.
         *
         *  @param verts Reference to our @ref buffer_object filled with vertex data.
         */
        finalvao(const buffer_object& verts):ibo(nullptr), vertices(verts)
        {
            Create();
        };

        /**
         *  Indexed constructor.
         *
         *  @param ibo      Reference to our @ref buffer_object filled with vertex <i>indices</i> instead of vertex data.
         *  @param verts    Reference to our @ref buffer_object filled with vertex data.
         */
        finalvao(const buffer_object &ibo, const buffer_object& verts):ibo(&ibo), vertices(verts)
        {
            Create();
        };

        //No virtual dtor, because finalvao should not be stored directly, i.e. this should only ever be used as a reference.

    private:
        //Prevent dynamic instantiation since we have no virtual dtor.
        static void* operator new(std::size_t) = delete;
        static void* operator new[](std::size_t) = delete;
        static void operator delete(void*) = delete;
        static void operator delete[](void*) = delete;

    public:
        /**
         *  Create a Vertex Attribute Object
         */
        void Create();

        /**
         *  Realease our VAO.
         */
        void Release();

        /**
         *  Bind this VAO.
         */
        void Bind();

        /**
         * Unind this VAO.
         */
        void Unbind();

        /**
         * Get the ID of this VAO.
         *
         *  @return @ref id of this VAO.
         */
        GLuint GetID() const {return id;}

        /**
         *  Draw our VAO.
         */
        void Draw();

    protected:
        GLuint  id;                         /**< Our VAO's identification number. **/
        const buffer_object& vertices;      /**< Vertex Buffer Object */

        //FIXME: std::optional<buffer_object&> would be nicer
        const buffer_object *const ibo;     /**< (Optional) Buffer for indexed vertices. **/
    };

    /**
     *  A VAO without indexed vertices.
     *
     *  You probably want to use @ref vao.
     *
     *  @tparam numbuffers Number of buffers that should be reserved in @ref buffers.
     */
    template<std::size_t numbuffers>
    struct mutablevao : public finalvao
    {
    protected:

        /**
         *  Our VAO that inherits from @ref finalvao. Contains a list of all our @ref buffer_object structures.
         *
         *  @param vertices Reference to our vertices @ref buffer_object.
         *
         *  @note This should not be directly instantiated.
         */
        mutablevao(const buffer_object& vertices): finalvao(vertices) {}
        using finalvao::finalvao;

    public:
        /**
         *  Specifies data type of data passed to paramater 3 of glVertexAttribPointer.
         */
        enum class DataType
        {
            BYTE    = GL_BYTE,
            UBYTE   = GL_UNSIGNED_BYTE,
            SHORT   = GL_SHORT,
            USHORT  = GL_UNSIGNED_SHORT,
            INT     = GL_INT,
            UINT    = GL_UNSIGNED_INT,
            FLOAT   = GL_FLOAT
        };

        /**
         *  Set vertex shader location of data in VBO for specified attribute.
         *
         *  @param attrib   Attribute we want to map data input in our shader (using location = x).
         *  @param type     Data Type of data passed in.
         *  @param size     The size of each component per attribute.
         *  @param stride   Byte offset between vertex attributes.
         *  @param offset   Offset to attribute in buffer. Each attrib is then located at offset + stride.
         */
        void SetDataLocation(GLuint attrib, DataType type, GLint size, GLsizei stride, GLint offset);

        /**
         *  Overload the [] operator to get a buffer stored in @ref buffers more easily.
         *
         *  @param slot Index slot where the buffer is stored. Usually in a @ref Attribute.
         *
         *  @return buffer_object& reference to the buffer object stored in @ref buffers.
         */
        buffer_object& operator[] (GLuint slot)
        {
            return buffers[slot];
        }
    protected:
        std::array<buffer_object, numbuffers> buffers; /**< Our list of current VBOs. */

    private:

        /**
         *  Get offset (stride) into a buffer stored in @ref buffers.
         *
         *  @param offset Offset to the first element.
         *
         *  @return void* Pointer to the first element of stride @p offset.
         */
        static constexpr void* buffer_offset(GLint offset){return static_cast<char *>(nullptr) + offset;}
    };

    /**
     *  Set vertex shader location of data in VBO for specified attribute.
     *
     *  @tparam numbuffers  Number of buffers.
     *  @param attrib       Attribute we want to map data input in our shader (using location = x).
     *  @param type         Data Type of data passed in.
     *  @param size         The size of each component per attribute.
     *  @param stride       Byte offset between vertex attributes.
     *  @param offset       Offset to attribute in buffer. Each attrib is then located at offset + stride.
     */
    template<std::size_t numbuffers>
    void mutablevao<numbuffers>::SetDataLocation(GLuint attrib, DataType type, GLint size, GLsizei stride, GLint offset)
    {
        Bind();
        glEnableVertexAttribArray(attrib);
        ASSERT_MSG(attrib < numbuffers, "VBO index (attribute) out of range");
        buffers[attrib].Bind();
        glVertexAttribPointer(attrib, size, static_cast<GLenum>(type), GL_FALSE, stride, buffer_offset(offset));
    }

    /**
     *  A VAO with indexed vertices.
     *
     *  You probably want to use @ref vao.
     *
     *  @tparam numbuffers  Number of buffers that should be reserved in @ref buffers.
     *  @tparam index_slot  The slot number where our index @ref buffer_object resides.
     */
    template<std::size_t numbuffers, std::size_t index_slot>
    struct indexedmutablevao : public mutablevao<numbuffers>
    {
    protected:
        /**
         *  Constructor.
         *
         *  @param vertices Reference to our vertices @ref buffer_object.
         *
         *  @note This should not be directly instantiated.
         */
        indexedmutablevao(const buffer_object& vertices): mutablevao<numbuffers>(mutablevao<numbuffers>::buffers[index_slot], numbuffers, vertices) {}
    };

    /**
     *  Declares parent type for @ref vao.
     *
     *  Either @ref indexedmutablevao or @ref mutablevao (unindexed).
     *
     *  @tparam Attribute An attribute enum.
     *
     *  @p  Attribute should have a value @c MAX to indicate the number of attributes (slots).
     *      If @p Attribute has a value @c INDEX_SLOT, then the @ref type is an @ref indexedmutablevao.
     *
     *  @note This class cannot be instantiated!
     */
    template<typename Attribute>
    struct vaoparent final
    {
    private:
        template<typename T>
        static constexpr indexedmutablevao<T::MAX, T::INDEX_SLOT> test(decltype(T::INDEX_SLOT)*);

        template<typename T>
        static constexpr mutablevao<T::MAX> test(...);

    public:
        using type = decltype(test<Attribute>(0)); /**< Type of vao this is (either @ref mutablevao or @ref indexedmutablevao) */

    private:
        vaoparent() = delete;
    };

    /**
     *  Our VAO object. This handles all @ref buffer_objects needed to draw data that has been.
     *  sent to the graphics card.
     *
     *  @tparam Attribute VAO Attribute enum.
     *
     *  @p  Attribute should contain @c MAX to indicate how many attribute slots are required.
     *      If @p Attribute contains @c INDEX_SLOT, then @ref vaoparent::type is @ref indexedmutablevao.
     */
    template<typename Attribute>
    struct vao final : public vaoparent<Attribute>::type
    {
    public:
        using DataType = typename vaoparent<Attribute>::type::DataType;

        /**
         *  Constructor.
         */
        vao(): vaoparent<Attribute>::type(vaoparent<Attribute>::type::buffers[Attribute::VERTEX_SLOT]){}

        /**
         *  Set vertex shader location of data in VBO for specified attribute.
         *
         *  @param attrib   Attribute we want to map data input in our shader (using location = x).
         *  @param type     Data Type of data passed in.
         *  @param size     The size of each component per attribute.
         *  @param stride   Byte offset between vertex attributes.
         *  @param offset   Offset to attribute in buffer. Each attrib is then located at offset + stride.
         */
        void SetDataLocation(Attribute attrib, DataType type, GLint size, GLsizei stride, GLint offset)
        {
            ASSERT_MSG(attrib < Attribute::MAX, "VBO index (attribute) out of range");
            vaoparent<Attribute>::type::SetDataLocation(static_cast<GLuint>(attrib), type, size, stride, offset);
        }
    };
}

#endif // GLVERTARRAY_HPP_INCLUDED
