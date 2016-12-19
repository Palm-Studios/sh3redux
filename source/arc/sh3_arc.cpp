/*++

Copyright (c) 2016  Palm Studios and Mike M (@perdedork)

Module Name:
        sh3_arc.cpp

Abstract:
        Implementation of functions found in the class sh3_arc

Author:
        Quaker762

Environment:

Notes:

Revision History:
        17-12-2016: File Created [Quaker762]
        17-12-2016: Started an implementation of Load( )

--*/
#include <SH3/arc/sh3_arc_types.hpp>

const char* defaultPath = "/data/arc.arc";

//////////////////////////////////////////////////////////////////////////////

/*++

Routine Description:
        Open a handle to arc.arc and load each section

Arguments:
        path - Path to arc.arc

Return Type:
        int - Number of bytes read, otherwise

--*/
int sh3_arc::Load()
{
    gzFile  aHandle = NULL;
    int     res;

    if((aHandle = gzopen(defaultPath, "rb")) == NULL) // Open a handle to arc.arc
    {
        Log(LOG_FATAL, "E00001: sh3_arc::Load( ): Unable to find /data/arc.arc!");
        messagebox("Fatal Error", "E00001: sh3_arc::Load( ): Unable to find /data/arc.arc!");
        exit(-1); // Just exit, there's no point continuing.
    }

    // Now, we read in the first 16 bytes (the header) and make sure this really is arc.arc!
    if((res = gzread(aHandle, (voidp)&s_fileHeader, sizeof(sh3_arc_mft_header_t))) != sizeof(sh3_arc_mft_header_t))
    {
        Log(LOG_FATAL, "E00002: sh3_arc::Load( ): Error reading arc.arc header! Was the handle opened correctly?!");
        messagebox("Fatal Error", "E00002: sh3_arc::Load( ): Error reading arc.arc header! Was the handle opened correctly?!");
        exit(-1);
    }

    // Check the first 4-bytes of the header to make sure we're not about to read a whole lot of garbage!
    if(s_fileHeader.file_marker != ARCARC_MAGIC)
    {
        Log(LOG_FATAL, "E00003: sh3_arc::Load( ): arc.arc, Invalid File Marker!!!");
        messagebox("Fatal Error", "E00003: sh3_arc::Load( ): arc.arc, Invalid File Marker!!!");
        exit(-1);
    }

    /* Okay, so we now know that
         A. We have a valid file handle to arc.arc
         B. We have loaded and validated the header

       We can now begin loading gather info about arc.arc and begin loading
       each section (or as I like to call them, sub arcs).
    */

    if((res = gzread(aHandle, (voidp)&s_infoHeader, sizeof(sh3_arc_data_header_t))) != sizeof(sh3_arc_data_header_t))
    {
        Log(LOG_FATAL, "E00004: sh3_arc::Load( ): Invalid read of arc.arc information!");
        messagebox("Fatal", "E00004: sh3_arc::Load( ): Invalid read of arc.arc information!");
        exit(-1);
    }

    c_pSections = new sh3_arc_section[s_infoHeader.sectionCount];

    for(unsigned int i = 0; i < s_infoHeader.sectionCount; i++)
    {
        c_pSections[i].Load(aHandle);
    }

    gzclose(aHandle);
    return res;
}

