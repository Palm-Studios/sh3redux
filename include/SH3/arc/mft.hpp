/** @file
 *  Functions to extract data from @c arc.arc.
 *  
 *  @copyright 2016-2017  Palm Studios
 */
#ifndef SH3_ARC_MFT_HPP_INCLUDED
#define SH3_ARC_MFT_HPP_INCLUDED

#include <zlib.h>

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>

#include "SH3/error.hpp"

namespace sh3 { namespace arc {

    /**
     *  A struct to read data from the @c arc.arc.
     */
    struct mft final
    {
    private:
        /**
         *  zlib closer functor.
         */
        struct gz_file_closer final
        {
        public:
            void operator()(gzFile file) const { gzclose(file); }
        };

    public:
        /**
         *  An enum representing possible results from trying to read data.
         */
        enum class read_result
        {
            SUCCESS,      ///< Success.
            END_OF_FILE,  ///< Already reached the end of the file.
            PARTIAL_READ, ///< End of file reached before completing the read.
            GZ_ERROR,     ///< An error happened while decompressing.
        };

        /**
         *  The @ref error class for @ref read_result.
         */
        class read_error final : public error<read_result>
        {
        public:
            /**
             *  Set the wrapped @ref read_result while reading a @c gzFile.
             *  
             *  @param result The result to assign.
             *  @param file   The file that was read.
             *  
             *  If @p result is @ref read_result::GZ_ERROR then the <tt>file</tt>'s zlib error code is extracted.
             *  Otherwise, @p file may also be set to @c nullptr.
             */
            void set_error(read_result result, gzFile file);

            /**
             *  Stringify the @ref read_error.
             *  
             *  @returns The error as a @c std::string.
             */
            std::string message() const;

        private:
            int zlib_err = Z_OK;
            int os_err = 0;
        };

        /**
         *  Open the @c arc.arc.
         *  
         *  @param[in] path The path to the @c arc.arc.
         */
        explicit mft(const std::string& path);

        /**
         *  Check whether the file was successfully opened.
         *  
         *  @returns @c true if the file was successfully opened, @c false otherwise.
         */
        bool is_open() const {return static_cast<bool>(gzHandle);}

        /**
         *  Read binary data from an arc file to a buffer.
         *  
         *  @param[out] destination The destination buffer.
         *  @param      len         The number of bytes to read into @p destination.
         *  @param[out] e           The @ref read_error, which is set if any error occurs.
         *  
         *  @returns The number of bytes read.
         *  
         *  @see @ref ReadObject
         *  @see @ref ReadString
         */
        std::size_t ReadData(void* destination, std::size_t len, read_error& e);

        /**
         *  Read binary data from an arc file to an object.
         *  
         *  @tparam T The type of the object to read into.
         *  
         *  @param[out] destination The destination object.
         *  @param      len         The number of bytes to read into @p destination.
         *  @param[out] e           The @ref read_error, which is set if any error occurs.
         *  
         *  @returns The number of bytes read.
         *  
         *  The @p destination must be a trivially copyable type so that it can be serialized directly from binary data.
         *  
         *  @see @ref ReadObject(T& destination, read_error& e)
         *  @see @ref ReadData
         *  @see @ref ReadString
         */
        template<typename T, typename = std::enable_if<std::is_trivially_copyable<T>::value>>
        size_t ReadObject(T& destination, std::size_t len, read_error& e)
        {
            return ReadData(&destination, len, e);
        }

        /**
         *  Read binary data from an arc file to an object.
         *  
         *  @tparam T The type of the object to read into.
         *  
         *  @param[out] destination The destination object.
         *  @param[out] e           The @ref read_error, which is set if any error occurs.
         *  
         *  @returns The number of bytes read.
         *  
         *  The @p destination must be a trivially copyable type so that it can be serialized directly from binary data.
         *  
         *  @see @ref ReadObject(T& destination, std::size_t len, read_error& e)
         *  @see @ref ReadData
         *  @see @ref ReadString
         */
        template<typename T, typename = std::enable_if<std::is_trivially_copyable<T>::value>>
        size_t ReadObject(T& destination, read_error& e)
        {
            return ReadObject(destination, sizeof(destination), e);
        }

        /**
         *  Read a string from an arc file.
         *  
         *  @param[out] destination The string to read into.
         *  @param      len         The number of characters to read.
         *  @param[out] e           The @ref read_error, which is set if any error occurs.
         *  
         *  @returns The number of bytes read.
         *  
         *  @see @ref ReadObject
         *  @see @ref ReadString
         */
        std::size_t ReadString(std::string& destination, std::size_t len, read_error& e);

    private:
        std::unique_ptr<gzFile_s, gz_file_closer> gzHandle;
    };

} }

#endif // SH3_ARC_MFT_HPP_INCLUDED
