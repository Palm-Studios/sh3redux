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

namespace sh3 { namespace arc {

    struct mft final
    {
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
         *
         *  @returns  The file length if loading is successful, @ref arcFileNotFound if not.
         */
        int LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start);

        /**
         *  Load a file from an subarc into @c buffer.
         *
         *  The contents of the file will be appended to @c buffer.
         *  The @c buffer will be resized if necessary.
         *
         *  @param filename Path to the file to load.
         *  @param buffer   The buffer to store the file contents into.
         *
         *  @returns The file length if loading is successful, @ref arcFileNotFound if not.
         */
        int LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer) { auto back = end(buffer); return LoadFile(filename, buffer, back); }
    };

} }

#endif // SH3_ARC_MFT_HPP_INCLUDED
