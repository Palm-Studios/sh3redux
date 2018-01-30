/** @file
 *  Describes the structures necessary to load/read Silent Hill 3 sub-arc files.
 *
 *  @see @ref arc-files
 *
 *  @copyright 2016-2017 Palm Studios and Mike M (<a href="https://twitter.com/perdedork">\@perdedork</a>)
 *
 *  @date 14-12-2016
 *
 *  @author Jesse Buhagiar
 */
#ifndef SH3_SUBARC_HPP_INCLUDED
#define SH3_SUBARC_HPP_INCLUDED

#include <cstdint>
#include <ios>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "SH3/error.hpp"

namespace sh3 { namespace arc {
    static constexpr int arcFileNotFound = -1; /**< Status @ref mft::LoadFile() and @ref subarc::LoadFile() return if a file cannot be found. */

    /**
     *  An sub-arc.
     */
    class subarc final
    {
    public:
        /** Index to retrieve a file. */
        using index_t = std::uint16_t;
        /** A mapping of filenames to the file's @ref index_t. */
        using files_map = std::map<std::string, index_t>;

        /**
         *  An enum representing possible results from trying to load a file.
         */
        enum class load_result
        {
            SUCCESS,             ///< Success.
            FILE_NOT_FOUND,      ///< File not found.
            SUBARC_NOT_FOUND,    ///< Subarc not found.
            SUBARC_ERROR,        ///< The subarc could not be parsed correctly.
            //TODO: END_OF_FILE?
            PARTIAL_READ,        ///< End of file reached before completing the load.
            PARTIAL_HEADER_READ, ///< End of file reached before even loading the file.
        };
        struct load_error final : public error<load_result>
        {
        public:
            /**
             *  Types of header for which @ref load_result::PARTIAL_HEADER_READ can occur.
             */
            enum class header
            {
                SUBARC, ///< The @ref subarc_header.
                FILE    ///< The @ref subarc_file_entry.
            };

        public:
            void set_error(load_result result) = delete;

            /**
             *  Set the wrapped @ref load_result to @ref load_result::SUBARC_NOT_FOUND.
             */
            void set_subarc_not_found_error() { result = load_result::SUBARC_NOT_FOUND; }

            /**
             *  Set the wrapped @ref load_result to @ref load_result::FILE_NOT_FOUND.
             */
            void set_file_not_found_error() { result = load_result::FILE_NOT_FOUND; }

            /**
             *  Set the wrapped @ref load_result to @ref load_result::SUBARC_ERROR.
             *  
             *  @param readMagic The magic value read from the header.
             */
            void set_subarc_error(std::uint32_t readMagic) { result = load_result::SUBARC_ERROR; magic = readMagic; }

            /**
             *  Set the wrapped @ref load_result to @ref load_result::PARTIAL_READ.
             *  
             *  @param fileSize The size of the file.
             *  @param numRead  The number of bytes that could be retrieved.
             */
            void set_partial_read_error(std::size_t fileSize, std::size_t numRead) { result = load_result::PARTIAL_READ; readError = {fileSize, numRead}; }

            /**
             *  Set the wrapped @ref load_result to @ref load_result::PARTIAL_HEADER_READ.
             *  
             *  @param headerType The type of the header.
             *  @param numRead    The number of bytes that could be retrieved.
             */
            void set_partial_header_read_error(header headerType, std::size_t numRead) { result = load_result::PARTIAL_HEADER_READ; headerReadError = {headerType, numRead}; }

            /**
             *  Stringify the @ref load_error.
             *  
             *  @returns The error as a @c std::string.
             */
            std::string message() const;

        private:
            /**
             *  Additional data for @ref load_result::PARTIAL_READ.
             */
            struct read_error
            {
                std::size_t size; ///< The number of bytes that were supposed to be read.
                std::size_t read; ///< The number of bytes read.
            };
            /**
             *  Additional data for @ref load_result::PARTIAL_HEADER_READ.
             */
            struct header_read_error
            {
                header headerType; ///< The type of the header.
                std::size_t read;  ///< The number of bytes read.
            };
            union
            {
                std::uint32_t magic; ///< Additional data for @ref load_result::SUBARC_ERROR.
                read_error readError; ///< See @ref read_error.
                header_read_error headerReadError; ///< See @ref header_read_error.
            };
        };

    public:
        subarc(subarc&&) = default;
        /** Constructor.
         *  
         *  @param subarcName The name of this @ref subarc.
         *  @param filesMap   The @ref files_map for this @ref subarc.
         */
        subarc(std::string &&subarcName, files_map &&filesMap): name(std::move(subarcName)), files(std::move(filesMap)) { }

        /**
         *  Load a file into @c buffer.
         *  
         *  The @c buffer will be resized if necessary.
         *  
         *  @param filename Path to the file to load.
         *  @param buffer   The buffer to store the file contents into.
         *  @param start    An iterator to the insertion position in @c buffer.
         *  @param e        @ref load_error from this operation.
         *  
         *  @returns The file length if loading is successful, @ref arcFileNotFound if not.
         */
        std::size_t LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start, load_error &e);

        /**
         *  Load a file into @c buffer.
         *  
         *  The @c buffer will be resized if necessary.
         *  
         *  @param filename Path to the file to load.
         *  @param buffer   The buffer to store the file contents into.
         *  @param e        @ref load_error from this operation.
         *  
         *  @returns The file length if loading is successful, @ref arcFileNotFound if not.
         */
        std::size_t LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, load_error &e) { auto back = end(buffer); return LoadFile(filename, buffer, back, e); }

        /**
         *  Load a file into @c buffer.
         *  
         *  The @c buffer will be resized if necessary.
         *  
         *  @param index  The @ref index_t for the file to load.
         *  @param buffer The buffer to store the file contents into.
         *  @param start  An iterator to the insertion position in @c buffer.
         *  @param e        @ref load_error from this operation.
         *  
         *  @returns The file length if loading is successful, @ref arcFileNotFound if not.
         */
        std::size_t LoadFile(index_t index, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start, load_error &e);

        /**
         *  Load a file into @c buffer.
         *  
         *  The @c buffer will be resized if necessary.
         *  
         *  @param index  The @ref index_t for the file to load.
         *  @param buffer The buffer to store the file contents into.
         *  @param e        @ref load_error from this operation.
         *  
         *  @returns The file length if loading is successful, @ref arcFileNotFound if not.
         */
        std::size_t LoadFile(index_t index, std::vector<std::uint8_t>& buffer, load_error &e) { auto back = end(buffer); return LoadFile(index, buffer, back, e); }

    public:
        const std::string name; /**< Name of this subarc. */

    private:
        /** Open the subarc-file.
         *  
         *  @param mode The @c openmode for the file.
         *  
         *  @returns The subarc-file stream.
         */
        std::ifstream open(std::ios_base::openmode mode = std::ios::binary);


        /** Maps a file (and its associated virtual path) to its subarc index. */
        files_map files;
    };

} }

#endif //SH3_SUBARC_HPP_INCLUDED
