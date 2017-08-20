/** @file
 *  Describes the structures necessary to load/read Silent Hill 3 sub-arc files.
 *
 *  @see @ref arc-files
 *
 *  @copyright 2016-2017 Palm Studios and Mike M (<a href="https://twitter.com/perdedork">\@perdedork</a>)
 *
 *  @date 14-12-2016
 *
 *  @author Jesse Buhagiar
 */
#ifndef SH3_ARC_TYPES_HPP_INCLUDED
#define SH3_ARC_TYPES_HPP_INCLUDED

#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <zlib.h>

#define ARC_FILE_NOT_FOUND  -1 /**< Status @ref sh3_arc::LoadFile() returns if a file cannot be found */

namespace sh3 { namespace arc {
    struct mft;
} }

/** @defgroup arc-headers arc header types
 *  @{
 */

/** Info about a section */
typedef struct
{
    uint16_t type;      /**< This is 2 (for an arc section header) */
    uint16_t hsize;     /**< Size of this header in bytes */
    uint32_t numFiles;  /**< Number of files in this section */
} sh3_arc_section_header_t;

/** Info about a file in a section */
typedef struct
{
    uint16_t type;          /**< This is 3 (for a file entry) */
    uint16_t fileSize;      /**< Size of this file entry(in bytes) */
    uint16_t arcIndex;      /**< Index of this file (in the .arc it is located in) */
    uint16_t sectionIndex;  /**< Index of the current section we are in. */
} sh3_arc_file_header_t;

/** Actual file entry */
typedef struct
{
    sh3_arc_file_header_t header;  /**< File info header */
    std::string           fname;   /**< File path and name (plus ext and @c NUL terminator) */
} sh3_arc_file_entry_t;

/** @} */

/**
 *  An arc section.
 */
class sh3_arc_section
{
public:
    sh3_arc_section_header_t          header;
    std::vector<sh3_arc_file_entry_t> fileEntries;

    std::string sectionName; /**< Name of this section */

    // Should this be private?!?!
    /** Maps a file (and its associated virtual path) to it's section index */
    std::map<std::string, std::uint32_t> fileList;
};

class sh3_arc
{
public:
    std::vector<sh3_arc_section> c_sections;    /**< List of all the sections in @c arc.arc */

    sh3_arc(){Load();}

    /**
     *  Initialize the @ref sh3_arc from the @c arc.arc file.
     *
     *  This initializes @ref s_fileHeader, @ref s_infoHeader and @ref c_sections.
     *
     *  @returns @c true if loading succeeded, @c false otherwise.
     */
    // should this be private?
    bool Load();

    /**
     *  Load a file from a section into @c buffer.
     *
     *  The @c buffer will be resized if necessary.
     *
     *  @param filename Path to the file to load.
     *  @param buffer   The buffer to store the file contents into.
     *  @param start    An iterator to the insertion position in @c buffer.
     *
     *  @returns  The file length if loading is successful, @c ARC_FILE_NOT_FOUND if not.
     */
    int LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start);

    /**
     *  Load a file from a section into @c buffer.
     *
     *  The contents of the file will be appended to @c buffer.
     *  The @c buffer will be resized if necessary.
     *
     *  @param filename Path to the file to load.
     *  @param buffer   The buffer to store the file contents into.
     *
     *  @returns The file length if loading is successful, @ref ARC_FILE_NOT_FOUND if not.
     */
    int LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer) { auto back = end(buffer); return LoadFile(filename, buffer, back); }
};

#endif // SH3_ARC_TYPES_HPP_INCLUDED
