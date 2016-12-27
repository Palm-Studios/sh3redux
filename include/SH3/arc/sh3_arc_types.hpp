/*++

Copyright (c) 2016  Palm Studios and Mike M (@perdedork)

Module Name:
        sh3_arc_types.hpp

Abstract:
        Describes the structures necessary to load/reada Silent Hill 3 .arc file
        (including arc.arc, the master file table).

        The structure of the .arc system is as follows:

                    arc.arc (MFT)
                        |
                        | (File name translation to section:offset)
            /-----/-----|-----\-----\
        pic.arc  chrbg.arc msg.arc tex.arc

        The files branching from the MFT are NOT compressed, however, arc.arc is compressed using
        gzip compression format. arc.arc basically acts as a Master File Table (or rather a root directory).

        It is probably a good idea to (at launch) load the MFT and map each file in a vector<>
        so that we can quickly look up and load a file in a section without having to transverse the MFT everytime,
        though it can be made quicker by skipping sections (which is most likely how Konami implemented it).

        After we have a handle to arc.arc, we can now load each section (as Mike has designated them) or as I like
        to call them (and perhaps more appropriately), sub arcs. These are the files found in /data/ of a regular
        install of SILENT HILL 3 on the PC. These sections contain information about each file contained within,
        such as a Virtual File Path (e.g /data/pic/it/it_xxxx.tex, then translated to an offset), the offset
        within the section it is located in, as well as the index of the section (for reasons that elude me).

        It is currently unknown how the programmers at Konami specified which files should be loaded when, but
        considering the very quick load times, I would assume that each part of the game is in its own individual
        sub arc/section, as Mike seems to have not found

Author:
        Quaker762

Environment:

Notes:

Revision History:
        14-12-2016: File Created                                        [Quaker762]
        17-12-2016: Finished adding base types                          [Quaker762]
        22-12-2016: Added sh3_arc::LoadFile() prototype                 [Quaker762]

--*/
#ifndef SH3_ARC_TYPES_HPP_INCLUDED
#define SH3_ARC_TYPES_HPP_INCLUDED

#include <SH3/stdtype.hpp>
#include <cstdint>
#include <map>
#include <string>
#include <zlib.h>

#define ARCARC_MAGIC 0x20030417

//////////////////////////////FILE AND TYPE HEADERS////////////////////////////////////////////////

// Type check header (an unfortunate waste of space)
typedef struct
{
    uint32_t file_marker;   // File marker for arc.arc, this is ALWAYS 0x20030417
    uint32_t unused[3];     // 3 filler DWORDs
} sh3_arc_mft_header_t;

// Info about the MFT
typedef struct
{
    uint16_t type;          // This is 1 (for arc.arc info )
    uint16_t header_size;   // Size of this header
    uint32_t sectionCount;  // How many sections there are in the mft (i.e how many .arc files in /data/)
    uint32_t fileCount;     // Number of files in this section
} sh3_arc_data_header_t;

// Info about a section
typedef struct
{
    uint16_t type;      // This is 2 (for an arc section header)
    uint16_t hsize;     // Size of this header in bytes
    uint32_t numFiles;  // Number of files in this section
} sh3_arc_section_header_t;

// Info about a file in a section
typedef struct
{
    uint16_t type;          // This is 3 (for a file entry)
    uint16_t fileSize;      // Size of this file entry(in bytes)
    uint16_t arcIndex;      // Index of this file (in the .arc it is located in)
    uint16_t sectionIndex;  // Index of the current section we are in.
} sh3_arc_file_header_t;

// Actual file entry
typedef struct
{
    sh3_arc_file_header_t   header;     // File info header
    char*                   fname;      // File path and name (plus ext and NULL terminator)
} sh3_arc_file_entry_t;

/////////////////////////////////////////////////////////////////////////////////////////////////

class sh3_arc_section
{
public:
    sh3_arc_section_header_t    header;
    sh3_arc_file_entry_t**      fileEntries;

    sh3_arc_section(){};
    ~sh3_arc_section(){safedelete_arr(fileEntries);};

    char* sectionName; // Name of this section

    // Should this be private?!?!
    std::map<char*, uint32_t> fileList; // Maps a file (and its associated virtual path) to it's section index

    // FUNCTION DECLARATIONS
    int Load(gzFile fHandle);

};

class sh3_arc
{
public:
    sh3_arc_mft_header_t    s_fileHeader;   // First 16 bytes of the file. Contains the file signature
    sh3_arc_data_header_t   s_infoHeader;   // Information about the MFT
    sh3_arc_section*        c_pSections;    // List of all the sections in arc.arc

    sh3_arc(){};
    ~sh3_arc(){safedelete_arr(c_pSections);};

    // FUNCTION DECLARATIONS
    int Load();
    int LoadFile(char* filename, uint8_t* buffer);

};



#endif // SH3_ARC_TYPES_HPP_INCLUDED
