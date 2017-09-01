/*++

Copyright (c) 2016  Palm Studios and Mike M (@perdedork)

Module Name:
        sh3_arc.cpp

Abstract:
        Implementation of functions found in the class sh3_arc

Author:
        Jesse Buhagiar

Environment:

Notes:

Revision History:
        17-12-2016: File Created                                                [jbuhagiar]
        17-12-2016: Started an implementation of Load( )                        [jbuhagiar]
        22-12-2016: Finished Implementation of Load( )                          [jbuhagiar]
                    Started an implementation of LoadFile( )
        27-12-2016: Reworked LoadFile( )                                        [jbuhagiar]
                    Fixed an issue where program would segfault due to not finishing the
                    for loop and then trying to read from the handle
                    Added a few constants (no magic numbers in THIS Dojo!)

--*/
#include "SH3/arc/types.hpp"
#include "SH3/arc/section.hpp"
#include "SH3/arc/subarc.hpp"
#include "SH3/arc/mft.hpp"
#include "SH3/system/log.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <type_traits>
#include <utility>

using namespace sh3::arc;

bool sh3_arc::Load()
{
    mft file;

    // Load each sub-arc
    c_sections.resize(file.GetSectionCount());

    for(subarc& section : c_sections)
    {
        file.ReadNextSection(section);
    }

    return true;
}

int sh3_arc::LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start)
{
    std::uint32_t    index;
    subarc* section = nullptr;

    // Find what section the file is in
    for(subarc& candidate : c_sections)
    {
        auto files = candidate.files.equal_range(filename);
        if(files.first == files.second)
        {
            continue; // No filename found in this section, continue over
        }
        else // We've found it
        {
            section = &candidate;
            // files.first is the std::map iterator
            // files.first->second is the value of the entry the iterator is pointing at
            index = files.first->second;
            if(next(files.first) != files.second)
            {
                Log(LogLevel::WARN, "Multiple files with name %s exist.", filename.c_str());
            }
            break;
        }
    }

    if(!section)
    {
        return ARC_FILE_NOT_FOUND;
    }

    static_cast<void>(index); // avoid clang warning - index *is* initialized now

    std::string sectionPath = "data/" + section->name + ".arc";
    std::ifstream sectionFile(sectionPath, std::ios::binary);
    if(!sectionFile)
    {
        die("E00005: sh3_arc::LoadFile( ): Unable to open a handle to section, %s!", section->name.c_str());
    }

    // Read the actual file from the appropriate sub arc
    sh3_subarc_header_t header;
    sh3_subarc_file_t   fileEntry;

    // Read header to check validity
    sectionFile.read(reinterpret_cast<char*>(&header), sizeof(header));
    if(header.magic != ARCSECTION_MAGIC)
    {
        die("sh3_arc::LoadFile( ): Subarc [%s] magic is incorrect! (Perhaps the file is corrupt!?)", section->name.c_str());
    }

    // Seek to the file entry and read it
    sectionFile.seekg(index * sizeof(fileEntry));
    static_assert(std::is_trivially_copyable<decltype(fileEntry)>::value, "must be deserializable through char*");
    sectionFile.read(reinterpret_cast<char*>(&fileEntry), sizeof(fileEntry));

    auto space = distance(start, end(buffer));
    assert(space >= 0);
    if(space < fileEntry.length)
    {
        using size_type = std::remove_reference<decltype(buffer)>::type::size_type;
        buffer.resize(fileEntry.length - static_cast<size_type>(space));
        start = end(buffer) - fileEntry.length;
    }

    sectionFile.seekg(fileEntry.offset);
    static_assert(std::is_trivially_copyable<std::remove_reference<decltype(*start)>::type>::value, "must be deserializable through char*");
    sectionFile.read(reinterpret_cast<char*>(&*start), fileEntry.length);
    advance(start, fileEntry.length);

    //FIXME: use error_code pattern like mft::ReadFile
    return static_cast<int>(fileEntry.length);
}
