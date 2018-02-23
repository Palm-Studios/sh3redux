/**
 *	.arc data definitions
 */
#ifndef _TYPES_HPP_DEFINED_
#define _TYPES_HPP_DEFINED_

#include <cstdint>
#include <vector>

/** arc.arc stuff **/

#pragma pack(push, 1)

static constexpr uint32_t ARC_MAGIC = 0x20030417; /**< @c arc.arc file magic */
static constexpr uint32_t SECTION_MAGIC = 0x20030507; 

/**
 *	arc.arc related headers
 */

/**
 *	File header for arc.arc
 */
typedef struct 
{
	uint32_t magic;
	uint32_t unused[3];
} sh3_arc_file_header;

/**
 *	Information about all the .arc sections
 */
typedef struct 
{
	uint16_t header_type;	// Should be 1
	uint16_t header_size;
	uint32_t num_sections;
	uint32_t file_count;
} sh3_arc_data_header;

typedef struct
{
	uint16_t type;	// Should be 2
	uint16_t hsize;
	uint32_t name_count; // Number of filenames in this section
} sh3_arc_section_header;

typedef struct
{
	uint16_t type;	// Should be 3
	uint16_t entry_size; // Size of the file descriptor
	uint16_t findex; // Index of this file in the section it is located in
	uint16_t sindex; // Index of the section this file is in
} sh3_arc_file_entry_header;

/////////////////////////////////////////////////////////////////////////////////

typedef struct 
{
	sh3_arc_file_entry_header	header; // Descriptor for this file entry
	char*						fname;
} arc_file_entry;

typedef struct
{
	sh3_arc_section_header		header;
	char*						section_name;
	std::vector<arc_file_entry> section_files;
} arc_section;

typedef struct
{
	uint32_t magic;
	uint32_t num_files;
	uint32_t padding; // Number of padding bytes before data begins
	uint32_t filler;
} sh3_section_header;

typedef struct
{
	uint32_t offset;
	uint32_t file_id;
	uint32_t file_size;
	uint32_t file_size2;
} sh3_section_file_entry;


#pragma pack(pop)

#endif