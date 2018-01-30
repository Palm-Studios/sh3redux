/** @file
 *  Functions to extract data from @c arc.arc.
 *  
 *  @see @ref arc-files
 *  
 *  @copyright 2016-2017 Palm Studios
 */
#ifndef SH3_ARC_MFT_HPP_INCLUDED
#define SH3_ARC_MFT_HPP_INCLUDED

#include <cstdint>
#include <string>
#include <vector>

#include "SH3/arc/subarc.hpp"
#include "SH3/system/assert.hpp"

namespace sh3 { namespace arc {

    struct mft final
    {
    public:
        /**
         *  An enum representing possible results from trying to load a file.
         */
        enum class load_result
        {
            SUCCESS,             ///< Success.
            FILE_NOT_FOUND,      ///< File not found.
            SUBARC_ERROR,        ///< Error loading the file from the subarc.
        };
        struct load_error final : public error<load_result>
        {
        public:
            void set_error(load_result res) { ASSERT(res == load_result::FILE_NOT_FOUND); result = res; }
            void set_error(subarc::load_error e) { ASSERT(e.get_result() != subarc::load_result::FILE_NOT_FOUND); result = load_result::SUBARC_ERROR; subarcError = e; }

            std::string message() const;

        private:
            subarc::load_error subarcError;
        };

    public:
        std::vector<subarc> subarcs;    /**< List of all the subarcs in @c arc.arc */

        mft();

        /**
         *  Load a file from an subarc into @c buffer.
         *
         *  The @c buffer will be resized if necessary.
         *
         *  @param filename Path to the file to load.
         *  @param buffer   The buffer to store the file contents into.
         *  @param start    An iterator to the insertion position in @c buffer.
         *  @param e        @ref load_error from this operation.
         *
         *  @returns  The file length if loading is successful, @ref arcFileNotFound if not.
         */
        std::size_t LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start, load_error &e);

        /**
         *  Load a file from an subarc into @c buffer.
         *
         *  The contents of the file will be appended to @c buffer.
         *  The @c buffer will be resized if necessary.
         *
         *  @param filename Path to the file to load.
         *  @param buffer   The buffer to store the file contents into.
         *  @param e        @ref load_error from this operation.
         *
         *  @returns The file length if loading is successful, @ref arcFileNotFound if not.
         */
        std::size_t LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, load_error &e) { auto back = end(buffer); return LoadFile(filename, buffer, back, e); }
    };

} }

#endif // SH3_ARC_MFT_HPP_INCLUDED
