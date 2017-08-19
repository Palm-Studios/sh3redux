/** @file
 *  Describes the structures necessary to load/read Silent Hill 3 .arc files
 *  (including @c arc.arc, the master file table).
 *
 *   The structure of the .arc system is as follows:
 *
 *                   arc.arc (MFT)
 *                       |
 *                       | (File name translation to section name)
 *           /-----/-----|-----\-----\
 *          |     |      |      |     |
 *       pic.arc  chrbg.arc msg.arc tex.arc
 *          |         |        |       |    (Translation to section offset)
 *          |         |        |       |
 *         File      File     File    File
 *
 *   The files branching from the MFT are NOT compressed, however, @c arc.arc is compressed using
 *   gzip compression format. @c arc.arc basically acts as a Master File Table (or rather a root directory).
 *
 *   It is probably a good idea to (at launch) load the MFT and map each file in a map (see @ref sh3_arc_section::fileList)
 *   so that we can quickly look up and load a file in a section without having to transverse the MFT everytime,
 *   though it can be made quicker by skipping sections (which is most likely how Konami implemented it).
 *
 *   After we have a handle to @c arc.arc, we can now load each section (as Mike has designated them) or as I like
 *   to call them (and perhaps more appropriately), sub arcs. These are the files found in @c /data/ of a regular
 *   install of SILENT HILL 3 on the PC. These sections contain information about each file contained within,
 *   such as a Virtual File Path (e.g @c /data/pic/it/it_xxxx.tex, then translated to an offset), the offset
 *   within the section it is located in, as well as the index of the section (for reasons that elude me).
 *
 *   It is currently unknown how the programmers at Konami specified which files should be loaded when, but
 *   considering the very quick load times, I would assume that each part of the game is in its own individual
 *   sub arc/section, as Mike seems to have not found
 *
 *  @copyright 2016  Palm Studios and Mike M (<a href="https://twitter.com/perdedork">\@perdedork</a>)
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

#define ARCARC_MAGIC        0x20030417  /**< arc.arc file magic */
#define ARC_FILE_NOT_FOUND  -1          /**< Status @ref sh3_arc::LoadFile() returns if a file cannot be found */
#define ARC_NUM_SECTIONS    30          /**< Number of section arcs SILENT HILL 3 contatins. */

namespace sh3 { namespace arc {
    struct mft;
} }

/** @defgroup arc-headers arc header types
 *  @{
 */

/** Type check header
 *
 *  (an unfortunate waste of space)
 */
typedef struct
{
    uint32_t file_marker;  /**< File marker for arc.arc. This is ALWAYS @c 0x20030417 */
    uint32_t unused[3];    /**< 3 filler DWORDs */
} sh3_arc_mft_header_t;

/** Info about the MFT */
typedef struct
{
    uint16_t type;          /**< This is 1 (for arc.arc info) */
    uint16_t header_size;   /**< Size of this header */
    uint32_t sectionCount;  /**< How many sections there are in the mft. (i.e how many .arc files in @c /data/) */
    uint32_t fileCount;     /**< Number of files in this section */
} sh3_arc_data_header_t;

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

    /**
     *  Load a section from @c arc.arc.
     *
     *  @param fHandle The @ref sh3::arc::mft.
     *
     *  @returns @c true if loading succeeded, @c false otherwise.
     */
    bool Load(sh3::arc::mft& fHandle);
};

class sh3_arc
{
public:
    sh3_arc_mft_header_t         s_fileHeader;  /**< First 16 bytes of the file. Contains the file signature */
    sh3_arc_data_header_t        s_infoHeader;  /**< Information about the MFT */
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
