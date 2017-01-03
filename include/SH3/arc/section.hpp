/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_arc_section.hpp

Abstract:
        Related data structures for a sub arc

Author:
        Jesse Buhagiar

Environment:

Notes:

Revision History:
        25-12-2016: File Created                                                    [jbuhagiar]

--*/
#ifndef SH3_ARC_SECTION_HPP_INCLUDED
#define SH3_ARC_SECTION_HPP_INCLUDED

#include <cstdint>

#define ARCSECTION_MAGIC 0x20030507

typedef struct
{
    std::uint32_t magic;             // File magic number
    std::uint32_t numFiles;          // Number of files located in this sub .arc
    std::uint32_t dataPointer;       // Pointer to the beginning of the data section
    std::uint32_t unused;            // Unused DWORD
} sh3_subarc_header_t;

typedef struct
{
    std::uint32_t offset;            // Offset file resides at
    std::uint32_t fileID;            // FileID????
    std::uint32_t length;            // Length of this file (in bytes)
    std::uint32_t length2;
} sh3_subarc_file_t;

#endif // SH3_ARC_SECTION_HPP_INCLUDED
