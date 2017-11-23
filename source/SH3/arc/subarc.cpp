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

int subarc::LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start)
{
    auto matching = files.equal_range(filename);
    if(matching.first != matching.second)
    {
        return arcFileNotFound;
    }

    using std::next;
    // files.first is the std::map iterator
    auto match = matching.first;
    if(next(match) != matching.second)
    {
        Log(LogLevel::WARN, "Multiple files with name %s exist in subarc %s.", filename.c_str(), name.c_str());
    }
    // match->second is the value of the entry the iterator is pointing at
    return LoadFile(match->second, buffer, start);
}

int subarc::LoadFile(index_t index, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start)
{
    std::ifstream file = open();
    if(!file)
    {
        die("E00005: subarc::LoadFile( ): Unable to open a handle to section, %s!", name.c_str());
    }

    // Read the actual file from the appropriate sub arc
    subarc_header header;
    // Read header to check validity
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    static constexpr decltype(header.magic) magic = 0x20030507; /**< Magic number (first 4 bytes) of an subarc header */
    if(header.magic != magic)
    {
        die("subarc::LoadFile( ): Subarc [%s] magic is incorrect! (Perhaps the file is corrupt!?)", name.c_str());
    }

    // Seek to the file entry and read it
    subarc_file_entry fileEntry;
    ASSERT(index <= std::numeric_limits<std::streamoff>::max() / sizeof(fileEntry));
    file.seekg(static_cast<std::streamoff>(index * sizeof(fileEntry)));
    static_assert(std::is_trivially_copyable<decltype(fileEntry)>::value, "must be deserializable through char*");
    file.read(reinterpret_cast<char*>(&fileEntry), sizeof(fileEntry));

    auto space = distance(start, end(buffer));
    ASSERT(space >= 0);
    if(space < fileEntry.length)
    {
        using size_type = std::remove_reference<decltype(buffer)>::type::size_type;
        buffer.resize(fileEntry.length - static_cast<size_type>(space));
        start = end(buffer) - fileEntry.length;
    }

    file.seekg(fileEntry.offset);
    static_assert(std::is_trivially_copyable<std::remove_reference<decltype(*start)>::type>::value, "must be deserializable through char*");
    file.read(reinterpret_cast<char*>(&*start), fileEntry.length);
    advance(start, fileEntry.length);

    //FIXME: use error_code pattern like mft_reader::ReadFile
    return static_cast<int>(fileEntry.length);
}
