/*++

Copyright (c) 2016  Palm Studios and Mike M (@perdedork)

Module Name:
        sh3_arc_section.cpp

Abstract:
        Implementation of functions found in the class sh3_arc_section

Author:
        Jesse Buhagiar

Environment:

Notes:

Revision History:
        17-12-2016: File Created [jbuhagiar]

--*/
#include "SH3/arc/types.hpp"
#include "SH3/arc/mft.hpp"
#include "SH3/system/log.hpp"

using namespace sh3::arc;

/*++

Routine Description:
        Load a section from arc.arc

Arguments:
        arcFile - a sh3_arc_file

Return Type:
        bool

--*/

bool sh3_arc_section::Load(mft& arcFile)
{
    if(!arcFile.is_open())
    {
        die("E00005: sh3_arc_section::Load( ): Unable to acquire file handle!");
    }

    mft::read_error readError;
    arcFile.ReadObject(header, readError);
    if(readError)
    {
        die("E00006: sh3_arc_section::Load( ): Invalid read of arc.arc section: %s!", readError.message().c_str());
    }

    arcFile.ReadString(sectionName, header.hsize - sizeof(header), readError);
    if(sectionName.back() != '\0')
    {
        die("E00007: sh3_arc_section::Load( ): Garbage read when reading section name (NUL terminator missing): %s!", sectionName.c_str());
    }
    // remove trailing NUL
    // Some filenames seem to have multiple NULs.
    while(sectionName.back() == '\0')
    {
        sectionName.pop_back();
    }
    sectionName.shrink_to_fit();

    // We have now loaded information about the section, so we can start
    // reading in all the files located in it (not in full, obviously...)
    fileEntries.resize(header.numFiles);

    for(sh3_arc_file_entry_t& file : fileEntries)
    {
        arcFile.ReadObject(file.header, readError);

        arcFile.ReadString(file.fname, file.header.fileSize - sizeof(file.header), readError);
        if(file.fname.back() != '\0')
        {
            die("E00008: sh3_arc_section::Load( ): Garbage read when reading file name (NUL terminator missing): %s!", file.fname.c_str());
        }
        // remove trailing NUL
        // Some filenames seem to have multiple NULs.
        while(file.fname.back() == '\0')
        {
            file.fname.pop_back();
        }
        file.fname.shrink_to_fit();
        //Log(LogLevel::INFO, "Read file: %s", file->fname.c_str());

        fileList[file.fname] = file.header.arcIndex; // Map the file name to its subarc index
        //Log(LogLevel::INFO, "Added file to file list!");
    }

    return true;
}
