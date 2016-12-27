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
        17-12-2016: File Created                                                [Quaker762]
        17-12-2016: Started an implementation of Load( )                        [Quaker762]
        22-12-2016: Finished Implementation of Load( )                          [Quaker762]
                    Started an implementation of LoadFile( )

--*/
#include <SH3/arc/sh3_arc_types.hpp>
#include <SH3/arc/sh3_arc_section.hpp>

const char* defaultPath = "data/arc.arc";

//////////////////////////////////////////////////////////////////////////////

/*++

Routine Description:
        Open a handle to arc.arc and load each section

Arguments:
        path - Path to arc.arc

Return Type:
        int - Number of bytes read, otherwise exits

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
        messagebox("Fatal Error", "E00004: sh3_arc::Load( ): Invalid read of arc.arc information!");
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
    int         i;
    int         index;
    uint8_t*    buff;
    FILE*       sectionHandle;

    // Find what section the file is in
    for(i = 0; i < s_infoHeader.sectionCount; i++)
    {
        if(c_pSections[i].fileList.count(filename) != 1)
        {
            continue; // No filename found in this section, continue over
        }
        else // We've found it, get some info we need!
        {
            index = c_pSections[i].fileList[filename];
            if((sectionHandle = fopen(c_pSections[i].sectionName, "rb")) == NULL)
            {
                Log(LOG_FATAL, "E00005: sh3_arc::LoadFile( ): Unable to open a handle to section %s!", c_pSections[i].sectionName);
                messagebox("Fatal Error", "E00005: sh3_arc::LoadFile( ): Unable to open a handle to section %s!", c_pSections[i].sectionName);
                return 0;
            }

            break;
        }

        return -1;
    }

    /*
        We now read in the actual file from the appropriate sub arc
    */
    sh3_subarc_header_t header;
    sh3_subarc_file_t   fileEntry;

    fread(&header, 1, sizeof(sh3_subarc_header_t), sectionHandle); // Read in the header

    if(header.magic != ARCSECTION_MAGIC) // Validate the magic number to make sure we're really reading a .arc file!
    {
        Log(LOG_ERROR, "sh3_arc::LoadFile( ): Sub arc magic incorrect!!!");
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

