/*++

Copyright (c) 2016  Palm Studios and Mike M (@perdedork)

Module Name:
        sh3_arc_section.cpp

Abstract:
        Implementation of functions found in the class sh3_arc_section

Author:
        Quaker762

Environment:

Notes:

Revision History:
        17-12-2016: File Created [Quaker762]

--*/
#include <SH3/arc/sh3_arc_types.hpp>
#include <SH3/arc/sh3_arc_file.hpp>

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

    if(arcFile.ReadObject(header) != sh3_arc_file::read_result::Success)
    {
        die("E00006: sh3_arc_section::Load( ): Invalid read of arc.arc section!");
    }

    arcFile.ReadString(sectionName, header.hsize - sizeof(header));

    // We have now loaded information about the section, so we can start
    // reading in all the files located in it (not in full, obviously...)
    fileEntries.resize(header.numFiles);

    for(sh3_arc_file_entry_t& file : fileEntries)
    {
        arcFile.ReadObject(file.header);

        arcFile.ReadString(file.fname, file.header.fileSize - sizeof(file.header));
        //Log(LOG_INFO, "Read file: %s", file->fname.c_str());

        fileList[file.fname] = file.header.arcIndex; // Map the file name to its subarc index
        //Log(LOG_INFO, "Added file to file list!");
    }

    return true;
}