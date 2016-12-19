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

/*++

Routine Description:
        Load a section from arc.arc

Arguments:
        fHandle - Handle to arc.arc

Return Type:
        int

--*/
int sh3_arc_section::Load(gzFile fHandle)
{
    int res;

    if(fHandle == nullptr || fHandle == NULL)
    {
        Log(LOG_FATAL, "E00005: sh3_arc_section::Load( ): Unable to acquire file handle!");
        messagebox("Fatal", "E00005: sh3_arc_section::Load( ): Unable to acquire file handle!");
        exit(-1);
    }

    if((res = gzread(fHandle, (voidp)&header, sizeof(sh3_arc_section_header_t))) != sizeof(sh3_arc_section_header_t))
    {
        Log(LOG_FATAL, "E00006: sh3_arc_section::Load( ): Invalid read of arc.arc section!");
        messagebox("Fatal", "E00006: sh3_arc_section::Load( ): Invalid read of arc.arc section!");
        exit(-1);
    }

    sectionName = new char[header.hsize - sizeof(sh3_arc_section_header_t)]; // Allocate some memory for the section name
    gzread(fHandle, (voidp)sectionName, header.hsize - sizeof(sh3_arc_section_header_t)); // No error checking, how naughty ;)

    // We have now loaded information about the section, so we can start
    // reading in all the files located in it (not in full, obviously...)
    fileEntries = new sh3_arc_file_header_t*[header.numFiles];

    for(int i = 0; i < header.numFiles; i++)
    {
        sh3_arc_file_header_t* file = new sh3_arc_file_header_t;
        gzread(fHandle, (voidp)file, sizeof(sh3_arc_file_header_t));

        fileEntries[i] = file;
    }
}
