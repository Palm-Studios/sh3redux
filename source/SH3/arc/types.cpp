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

static const char* defaultPath = "data/arc.arc";

using namespace sh3::arc;

bool sh3_arc::Load()
{
    mft file(defaultPath);

    if(!file.is_open())
    {
        die("E00001: sh3_arc::Load( ): Unable to find /data/arc.arc!");
    }

    // Now, we read in the first 16 bytes (the header) and make sure this really is arc.arc!
    mft::read_error readError;
    file.ReadObject(s_fileHeader, readError);
    if(readError)
    {
        die("E00002: sh3_arc::Load( ): Error reading arc.arc header: %s! Was the handle opened correctly?!", readError.message().c_str());
    }

    // Check the first 4-bytes of the header to make sure we're not about to read a whole lot of garbage!
    if(s_fileHeader.file_marker != ARCARC_MAGIC)
    {
        die("E00003: sh3_arc::Load( ): arc.arc, Invalid File Marker!!!");
    }

    /* Okay, so we now know that
         A. We have a valid file handle to arc.arc
         B. We have loaded and validated the header

       We can now begin loading gather info about arc.arc and begin loading
       each section (or as I like to call them, sub arcs).
    */

    file.ReadObject(s_infoHeader, readError);
    if(readError)
    {
        die("E00004: sh3_arc::Load( ): Invalid read of arc.arc information: %s!", readError.message().c_str());
    }

    c_sections.resize(s_infoHeader.sectionCount);

    for(sh3_arc_section& section : c_sections)
    {
        section.Load(file);
    }

    return true;
}

int sh3_arc::LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start)
{
    std::uint32_t    index;
    sh3_arc_section* section = nullptr;

    // Find what section the file is in
    for(sh3_arc_section& candidate : c_sections)
    {
        auto files = candidate.fileList.equal_range(filename);
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

    std::string sectionPath = "data/" + section->sectionName + ".arc";
    std::ifstream sectionFile(sectionPath, std::ios::binary);
    if(!sectionFile)
    {
        die("E00005: sh3_arc::LoadFile( ): Unable to open a handle to section, %s!", section->sectionName.c_str());
    }

    // Read the actual file from the appropriate sub arc
    sh3_subarc_header_t header;
    sh3_subarc_file_t   fileEntry;

    // Read header to check validity
    sectionFile.read(reinterpret_cast<char*>(&header), sizeof(header));
    if(header.magic != ARCSECTION_MAGIC)
    {
        die("sh3_arc::LoadFile( ): Subarc [%s] magic is incorrect! (Perhaps the file is corrupt!?)", section->sectionName.c_str());
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
