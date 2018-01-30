#include "SH3/arc/subarc.hpp"

#include <cstdint>
#include <fstream>
#include <iterator>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "SH3/system/assert.hpp"
#include "SH3/system/log.hpp"

using namespace sh3::arc;

/** @defgroup arc-section-headers arc section header types
 *  @{
 */

namespace {
     /**
      * .arc section header
      */
    struct subarc_header final
    {
        std::uint32_t magic;             /**< File magic number */
        std::uint32_t numFiles;          /**< Number of files located in this sub .arc */
        std::uint32_t dataPointer;       /**< Pointer to the beginning of the data section */
        std::uint32_t unused;            /**< Unused DWORD */

        static constexpr std::uint32_t expectedMagic = 0x20030507; /**< Expected value of @ref magic. */
    };

    /**
     *  File entry inside of an .arc section file [*.arc]
     */
    struct subarc_file_entry final
    {
        std::uint32_t offset;            /**< Offset file resides at */
        std::uint32_t fileID;            /**< FileID???? */
        std::uint32_t length;            /**< Length of this file (in bytes) */
        std::uint32_t length2;
    };
}

/** @}*/

std::ifstream subarc::open(std::ios_base::openmode mode)
{
    const std::string path = "data/" + name + ".arc";
    return std::ifstream(path, mode);
}
 
std::string subarc::load_error::message() const
{
    std::string error;
    switch(result)
    {
    case load_result::SUCCESS:
        error = "Success";
        break;
    case load_result::SUBARC_NOT_FOUND:
        error = "Subarc not accessible";
        break;
    case load_result::FILE_NOT_FOUND:
        error = "File not found";
        break;
    case load_result::SUBARC_ERROR:
        error = "Subarc magic mismatch (expected " + std::to_string(subarc_header::expectedMagic) + " got " + std::to_string(magic) + ")";
        break;
    case load_result::PARTIAL_READ:
        error = "Read " + std::to_string(readError.read) + "/" + std::to_string(readError.size) + " bytes before encountering an I/O error";
        break;
    case load_result::PARTIAL_HEADER_READ:
        {
            std::size_t size;
            std::string headerName;
            if(headerReadError.headerType == header::SUBARC)
            {
                size = sizeof(subarc_header);
                headerName = "subarc_header";
            }
            else
            {
                ASSERT(headerReadError.headerType == header::FILE);
                size = sizeof(subarc_file_entry);
                headerName = "subarc_file_entry";
            }
            error = "Read " + std::to_string(readError.read) + "/" + std::to_string(size) + " bytes for " + headerName;
        }
        break;
    };
    return error;
}

std::size_t subarc::LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start, load_error &e)
{
    using std::next;

    auto matching = files.equal_range(filename);
    if(matching.first == matching.second)
    {
        e.set_file_not_found_error();
        return 0;
    }

    using std::next;
    // files.first is the std::map iterator
    auto match = matching.first;
    if(next(match) != matching.second)
    {
        Log(LogLevel::WARN, "Multiple files with name %s exist in subarc %s.", filename.c_str(), name.c_str());
    }
    // match->second is the value of the entry the iterator is pointing at
    return LoadFile(match->second, buffer, start, e);
}

std::size_t subarc::LoadFile(index_t index, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start, load_error &e)
{
    std::ifstream file = open();
    if(!file)
    {
        e.set_subarc_not_found_error();
        return 0;
    }

    // Read the actual file from the appropriate sub arc
    subarc_header header;
    // Read header to check validity
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if(file.gcount() != sizeof(header))
    {
        ASSERT(file.fail());
        e.set_partial_header_read_error(load_error::header::SUBARC, static_cast<std::size_t>(file.gcount()));
        return 0;
    }
    if(header.magic != subarc_header::expectedMagic)
    {
        //TODO: report read magic
        e.set_subarc_error(header.magic);
        return 0;
    }

    // Seek to the file entry and read it
    subarc_file_entry fileEntry;
    ASSERT(index <= std::numeric_limits<std::streamoff>::max() / sizeof(fileEntry));
    file.seekg(static_cast<std::streamoff>(index * sizeof(fileEntry)), std::ios_base::cur);
    static_assert(std::is_trivially_copyable<decltype(fileEntry)>::value, "must be deserializable through char*");
    file.read(reinterpret_cast<char*>(&fileEntry), sizeof(fileEntry));
    if(file.gcount() != sizeof(fileEntry))
    {
        ASSERT(file.fail());
        e.set_partial_header_read_error(load_error::header::FILE, static_cast<std::size_t>(file.gcount()));
        return 0;
    }

    const auto prespace = distance(start, end(buffer));
    ASSERT(prespace >= 0);
    if(prespace < fileEntry.length)
    {
        using size_type = std::remove_reference<decltype(buffer)>::type::size_type;
        buffer.resize(buffer.size() - static_cast<size_type>(prespace) + fileEntry.length);
        start = end(buffer) - fileEntry.length;
    }

    file.seekg(fileEntry.offset);
    static_assert(std::is_trivially_copyable<std::remove_reference<decltype(*start)>::type>::value, "must be deserializable through char*");
    file.read(reinterpret_cast<char*>(&*start), fileEntry.length);
    std::size_t read = static_cast<std::size_t>(file.gcount());
    ASSERT(read <= fileEntry.length);
    advance(start, read);
    if(read != fileEntry.length)
    {
        ASSERT(file.fail());
        e.set_partial_read_error(read, fileEntry.length);

        //undo buffer resizing, if we did resize it before
        if(prespace < fileEntry.length)
        {
            buffer.resize(buffer.size() - (fileEntry.length - read));
        }
    }

    return read;
}
