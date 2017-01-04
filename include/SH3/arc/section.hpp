/** @file
 *  Related data structures for a sub arc
 *
 *  @copyright 2016  Palm Studios and Mike M (<a href="https://twitter.com/perdedork">\@perdedork</a>)
 *
 *  @date 25-12-2016
 *
 *  @author Jesse Buhagiar
 */
#ifndef SH3_ARC_SECTION_HPP_INCLUDED
#define SH3_ARC_SECTION_HPP_INCLUDED

#include <cstdint>

#define ARCSECTION_MAGIC 0x20030507 /**< Magic number (first 4 bytes) of an .arc section */

/** @defgroup arc-section-headers arc section header types
 *  @{
 */

 /**
  * .arc section header
  */
typedef struct
{
    std::uint32_t magic;             /**< File magic number */
    std::uint32_t numFiles;          /**< Number of files located in this sub .arc */
    std::uint32_t dataPointer;       /**< Pointer to the beginning of the data section */
    std::uint32_t unused;            /**< Unused DWORD */
} sh3_subarc_header_t;

/**
 *  File entry inside of an .arc section file [*.arc]
 */
typedef struct
{
    std::uint32_t offset;            /**< Offset file resides at */
    std::uint32_t fileID;            /**< FileID???? */
    std::uint32_t length;            /**< Length of this file (in bytes) */
    std::uint32_t length2;
} sh3_subarc_file_t;

/** @}*/

#endif // SH3_ARC_SECTION_HPP_INCLUDED
