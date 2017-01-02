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
#include "SH3/arc/file.hpp"
#include "SH3/system/log.hpp"

/*++

Routine Description:
        Load a section from arc.arc

Arguments:
        arcFile - a sh3_arc_file

Return Type:
        bool

--*/

bool sh3_arc_section::Load(sh3_arc_file& arcFile)
{
    if(!arcFile.is_open())
    {
        die("E00005: sh3_arc_section::Load( ): Unable to acquire file handle!");
    }

    sh3_arc_file::read_error readError;
    arcFile.ReadObject(header, readError);
    if(readError)
    {
        die("E00006: sh3_arc_section::Load( ): Invalid read of arc.arc section: %s!", readError.message().c_str());
    }

    arcFile.ReadString(sectionName, header.hsize - sizeof(header), readError);

    // We have now loaded information about the section, so we can start
    // reading in all the files located in it (not in full, obviously...)
    fileEntries.resize(header.numFiles);

    for(sh3_arc_file_entry_t& file : fileEntries)
    {
        arcFile.ReadObject(file.header, readError);

        arcFile.ReadString(file.fname, file.header.fileSize - sizeof(file.header), readError);
        //Log(LogLevel::Info, "Read file: %s", file->fname.c_str());

        fileList[file.fname] = file.header.arcIndex; // Map the file name to its subarc index
        //Log(LogLevel::Info, "Added file to file list!");
    }

    return true;
}
