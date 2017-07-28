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

#include <array>
#include <utility>
#include <vector>

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
        using Targets = std::array<buffer_object::Target, numbuffers>;

        /**
         *  Our VAO that inherits from @ref finalvao. Contains a list of all our @ref buffer_object structures.
         *
         *  @param targets  An array of @ref buffer_object::Target%s for the VBOs.
         *  @param vertices Reference to our vertices @ref buffer_object.
         *
         *  @note This should not be directly instantiated.
         */
        mutablevao(const Targets& targets, const buffer_object& vertices): mutablevao(targets, vertices, std::make_index_sequence<numbuffers>()) {}
        using finalvao::finalvao;

    private:
        template<std::size_t... seq>
        mutablevao(const Targets& targets, const buffer_object& vertices, std::index_sequence<seq...>): finalvao(vertices), buffers({targets[seq]...}) {}

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
         *  @param slot     Slot of attribute we want to map data input in our shader (using location = x).
         *  @param type     Data Type of data passed in.
         *  @param size     The size of each component per attribute.
         *  @param stride   Byte offset between vertex attributes.
         *  @param offset   Offset to attribute in buffer. Each attrib is then located at offset + stride.
         */
        void SetDataLocation(GLuint slot, DataType type, GLint size, GLsizei stride, GLint offset);

        /**
         *  Overload the [] operator to get a buffer stored in @ref buffers more easily.
         *
         *  @param slot Index slot where the buffer is stored. The value usually comes from a @c Attributes::Slot.
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

    template<std::size_t numbuffers>
    void mutablevao<numbuffers>::SetDataLocation(GLuint slot, DataType type, GLint size, GLsizei stride, GLint offset)
    {
        Bind();
        glEnableVertexAttribArray(slot);
        ASSERT_MSG(slot < numbuffers, "VBO index (slot) out of range");
        buffers[slot].Bind();
        glVertexAttribPointer(slot, size, static_cast<GLenum>(type), GL_FALSE, stride, buffer_offset(offset));
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
         *  @param targets  An array of @ref buffer_object::Target%s for the VBOs.
         *  @param vertices Reference to our vertices @ref buffer_object.
         *
         *  @note This should not be directly instantiated.
         */
        indexedmutablevao(const typename mutablevao<numbuffers>::Targets& targets, const buffer_object& vertices): mutablevao<numbuffers>(mutablevao<numbuffers>::buffers[index_slot], numbuffers, vertices), mutablevao<numbuffers>::buffers(targets) {}
    };

    /**
     *  Declares parent type for @ref vao.
     *
     *  Either @ref indexedmutablevao or @ref mutablevao (unindexed).
     *
     *  @tparam Slot A VAO-slot enum.
     *
     *  @p Slot must have a value named @c MAX to indicate how many attribute slots there are.
     *  If @p Slot has a value @c INDEX, then the @ref type is an @ref indexedmutablevao, otherwise it's @ref mutablevao.
     *
     *  @note This class cannot be instantiated!
     */
    template<typename Slot>
    struct vaoparent final
    {
    private:
        template<typename T>
        static constexpr indexedmutablevao<static_cast<std::size_t>(T::MAX), static_cast<std::size_t>(T::INDEX)> test(decltype(T::INDEX_SLOT)*);

        template<typename T>
        static constexpr mutablevao<static_cast<std::size_t>(T::MAX)> test(...);

    public:
        using type = decltype(test<Slot>(std::declval<Slot>())); /**< Type of vao this is (either @ref mutablevao or @ref indexedmutablevao) */

    private:
        vaoparent() = delete;
    };

    /**
     *  Type of an @c Attributes::Targets array.
     *  
     *  @see @ref sh3_gl::vao
     *  
     *  @tparam Slot The @c Attributes::Slot enum.
     *  
     *  @p Slot must have a value named @c MAX to indicate how many attribute slots there are.
     */
    template<typename Slot>
    using vao_target_array = std::array<buffer_object::Target, static_cast<std::size_t>(Slot::MAX)>;

    /**
     *  Our VAO object. This handles all @ref buffer_object%s needed to draw data that has been.
     *  sent to the graphics card.
     *
     *  @tparam Attributes VAO Attributes struct.
     *
     *  @p Attributes should contain two members, @c Slot and @c Targets.
     *  @c Slot should be an enum with consecutive symbolic constants for the attribute slots. There must be a VBO for the vertices, the slot of which must be named @c VERTEX. @c Slot must also have a value named @c MAX to indicate how many attribute slots there are in total.
     *  If @c Slot contains a value named @c INDEX, then this @ref vao uses the VBO with that slot as index for the @c VERTEX VBO.
     */
    template<typename Attributes>
    struct vao final : public vaoparent<enum Attributes::Slot>::type
    {
    private:
        using vaoparent = typename vaoparent<enum Attributes::Slot>::type;

    public:
        using DataType = typename vaoparent::DataType;
        using Slot = enum Attributes::Slot;

        /**
         *  Constructor.
         */
        vao(): vaoparent(Attributes::Targets, vaoparent::buffers[static_cast<std::size_t>(Slot::VERTEX)]){}

        /**
         *  Set vertex shader location of data in VBO for specified attribute.
         *
         *  @param slot     Slot of attribute we want to map data input in our shader (using location = x).
         *  @param type     Data Type of data passed in.
         *  @param size     The size of each component per attribute.
         *  @param stride   Byte offset between vertex attributes.
         *  @param offset   Offset to attribute in buffer. Each attrib is then located at offset + stride.
         */
        void SetDataLocation(Slot slot, DataType type, GLint size, GLsizei stride, GLint offset)
        {
            ASSERT_MSG(slot < Slot::MAX, "VBO index (slot) out of range");
            vaoparent::SetDataLocation(static_cast<GLuint>(slot), type, size, stride, offset);
        }

        buffer_object& operator[](Slot slot)
        {
            ASSERT_MSG(slot < Slot::MAX, "VBO index (slot) out of range");
            return vaoparent::operator[](static_cast<GLuint>(slot));
        }
    };
}

#endif // GLVERTARRAY_HPP_INCLUDED
