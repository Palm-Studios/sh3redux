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
#include "SH3/arc/file.hpp"

#include <algorithm>
#include <iterator>
#include <utility>

const char* defaultPath = "data/arc.arc";

//////////////////////////////////////////////////////////////////////////////

/*++

Routine Description:
        Open a handle to arc.arc and load each section

Arguments:
        path - Path to arc.arc

Return Type:
        bool

--*/
bool sh3_arc::Load()
{
    sh3_arc_file file(defaultPath);;

    if(!file.is_open())
    {
        die("E00001: sh3_arc::Load( ): Unable to find /data/arc.arc!");
    }

    // Now, we read in the first 16 bytes (the header) and make sure this really is arc.arc!
    if(file.ReadObject(s_fileHeader) != sh3_arc_file::read_result::Success)
    {
        die("E00002: sh3_arc::Load( ): Error reading arc.arc header! Was the handle opened correctly?!");
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

    if(file.ReadObject(s_infoHeader) != sh3_arc_file::read_result::Success)
    {
        die("E00004: sh3_arc::Load( ): Invalid read of arc.arc information!");
    }

    c_sections.resize(s_infoHeader.sectionCount);

    for(sh3_arc_section& section : c_sections)
    {
        section.Load(file);
    }

    return true;
}

/*++

Routine Description:
        Load a file from a section and store it in (an already allocated) buffer

Arguments:
        buffer* - Pre-alllocated buffer for file

Return Type:
        int - File length or 0 if none existent

--*/
int sh3_arc::LoadFile(char* filename, uint8_t* buffer)
{
    uint32_t         index;
    uint8_t*         buff;
    FILE*            sectionHandle;
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
            index = files.first->second;
            break;
        }
    }

    if(!section)
    {
        return ARC_FILE_NOT_FOUND;
    }

    if((sectionHandle = fopen(section->sectionName.c_str(), "rb")) == nullptr)
    {
        die("E00005: sh3_arc::LoadFile( ): Unable to open a handle to section, %s!", section->sectionName.c_str());
    }

    /*
        We now read in the actual file from the appropriate sub arc
    */
    sh3_subarc_header_t header;
    sh3_subarc_file_t   fileEntry;

    fread(&header, 1, sizeof(sh3_subarc_header_t), sectionHandle); // Read in the header

    if(header.magic != ARCSECTION_MAGIC) // Validate the magic number to make sure we're really reading a .arc file!
    {
        die("sh3_arc::LoadFile( ): Subarc [%s] magic is incorrect! (Perhaps the file is corrupt!?)", section->sectionName.c_str());
    }

    fseek(sectionHandle, index * sizeof(sh3_subarc_file_t), SEEK_CUR); // Seek to the file entry
    fread(&fileEntry, 1, sizeof(sh3_subarc_file_t), sectionHandle);

    buff = new uint8_t[fileEntry.length]; // Allocate new buffer

    fseek(sectionHandle, fileEntry.offset, SEEK_SET); // Seek to file Data
    fread(buff, 1, fileEntry.length, sectionHandle);

    memcpy(buffer, buff, fileEntry.length); // Copy to param buffer

    delete buff; // No memory leaks in THIS dojo!

    fclose(sectionHandle);

    return fileEntry.length;
}

