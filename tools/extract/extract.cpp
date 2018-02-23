/**
 *	Tool that dumps ALL files from all sections in /data/
 *
 *	@note This does NOT rely on the sh3r API
 *
 *	@date 21-2-2018
 *
 *	@author Jesse Buhagiar
 *
 *  @copyright 2016-2018  Palm Studios
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <zlib.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "types.hpp"
	
static std::vector<arc_section>			arc_sections; // Bad scoping, I know...
static uint32_t							num_sections;

void generate_file_table(gzFile& handle, bool verbose, bool dump)
{
	std::ofstream 				list;
	sh3_arc_file_header			mft_header;
	sh3_arc_data_header 		data_header;

	if(dump)
	{
		list.open("file_list.txt", std::ios::trunc);
		if(!list.is_open())
		{
			std::printf("Unable to open a handle to file_list.txt!!!\n");
		}	
	}

	// Walk the entirety of arc.arc
	gzread(handle, reinterpret_cast<voidp>(&mft_header), sizeof(sh3_arc_file_header));
	if(mft_header.magic != ARC_MAGIC)
	{
		std::printf("mft_header->magic != ARC_MAGIC\n");
		exit(-1);
	}

	gzread(handle, reinterpret_cast<voidp>(&data_header), sizeof(sh3_arc_data_header));
	arc_sections.resize(data_header.num_sections);
	num_sections = data_header.num_sections;

	if(verbose)
	{
		std::printf("arc.arc contains %d sections\n", static_cast<uint32_t>(arc_sections.size()));
	}

	for(unsigned int i = 0; i < data_header.num_sections; i++)
	{
		arc_section section;
		size_t 		name_size;

		gzread(handle, reinterpret_cast<voidp>(&(section.header)), sizeof(sh3_arc_section_header));
		name_size = section.header.hsize - sizeof(sh3_arc_section_header);
		section.section_name = new char[name_size]; // Don't judge me... It works.

		gzread(handle, reinterpret_cast<voidp>(section.section_name), name_size);

		if(verbose)
		{
			std::printf("Found section: %s\n", section.section_name);			
		}

		if(dump)
		{
			list << "Found section " << section.section_name << std::endl;
		}

		section.section_files.resize(section.header.name_count);


		// Now read all file entries from section and map them to an index
		for(int j = 0; j < section.header.name_count; j++)
		{
			arc_file_entry 	f;
			size_t			fname_size;

			gzread(handle, reinterpret_cast<voidp>(&(f.header)), sizeof(sh3_arc_file_entry_header));
			fname_size = f.header.entry_size - sizeof(sh3_arc_file_entry_header);
			f.fname = new char[fname_size];
			gzread(handle, f.fname, fname_size);

			section.section_files[j] = f;

			if(verbose)
			{
				std::printf("Found file: %s\n", section.section_files[j].fname);
				std::printf("\tSection Index: %d\n", f.header.sindex);
				std::printf("\tFile Index: %d\n\n", f.header.findex);
			}

			if(dump)
			{
				list << "Found file " << f.fname << std::endl;
				list << "\tSection Index: " << f.header.sindex << std::endl;
				list << "\tFile Index: " << f.header.findex << std::endl << std::endl;
			}
		}

		arc_sections[i] = section;
	}

	list.close();
}

void dump_arc_section(uint32_t sectionID, const std::string& p)
{
	arc_section 		section;
	std::string			section_path;
	std::ifstream 		section_file;
	sh3_section_header  header;



	section = arc_sections[sectionID];
	section_path = std::string("data/") + section.section_name + std::string(".arc");

	section_file.open(section_path.c_str(), std::ios_base::in | std::ios_base::binary);
	section_file.read(reinterpret_cast<char*>(&header), sizeof(sh3_section_header));

	std::printf("Extracting section: %s", section_path.c_str());
	if(header.magic != SECTION_MAGIC)
	{
		std::printf("header.magic != SECTION_MAGIC!\n");
		exit(-1);
	}

	for(unsigned int i = 0; i < header.num_files; i++)
	{
		sh3_section_file_entry 	entry;
		std::string 			filepath;
		std::vector<uint8_t> 	data;
		std::ofstream			output_file;

		section_file.seekg(sizeof(sh3_section_header) + (i * sizeof(sh3_section_file_entry)), std::ios_base::beg); // Seek to the correct entry
		section_file.read(reinterpret_cast<char*>(&entry), sizeof(sh3_section_file_entry));

		filepath = p + std::string(section.section_files[i].fname);

		// Check if the filepath actually exists on disk!
		boost::filesystem::path fpath(filepath.substr(0, filepath.find_last_of("/")));
		boost::system::error_code returnedError;
		if(!boost::filesystem::exists(fpath))
			boost::filesystem::create_directories(fpath, returnedError);

		data.resize(entry.file_size);
		section_file.seekg(entry.offset, std::ios_base::beg);
		section_file.read(reinterpret_cast<char*>(&data[0]), entry.file_size);

		// Create the file on disk
		output_file.open(filepath, std::ios::binary);
		output_file.write(reinterpret_cast<char*>(&data[0]), entry.file_size);
		output_file.close();

		// Cool loading indicator!
		if((i % 8) == 0)
		{
			std::fprintf(stdout, ".");
			std::fflush(stdout); // Flush buffer output
		}
	}

	std::printf("\n");
	section_file.close();
}

int main(int argc, char** argv)
{
	std::string arc_path = "data/";
	std::string out_path = "data/"; // Default to writing files to 'data/data/xyz'
	gzFile		arc_handle;
	bool 		mft_dump = false;
	bool		verbose = false;

	if(argc == 1)
	{
        std::printf("sh3r arc extraction utility. Copyright 2016-2018 Palm Studios\n\n");
        std::printf("No path detected!\n\n");
        std::printf("Usage: \t-p [path]: Path to where arc.arc is.\n");
        std::printf("       \t-v: Verbose output\n");
        std::printf("       \t-d: Dump arc.arc information\n");
        std::printf("       \t-o [path]: Directory to extract data to.\n");
		return -1;
	}
	else
	{
        for(int i = 1; i < argc; i++) // Walk the arguments list
        {
            if(argv[i][0] =='-') // We've found an argument
            {
                // Interpret argument
                switch(argv[i][1])
                {
                case 'p':
                	arc_path = argv[i+1];
                	i++;
                	break;
                case 'v':
                	verbose = true;
                	i++;
                	break;
                case 'd':
                	mft_dump = true;
                	i++;
                	break;
                case 'o':
                	out_path = argv[i+1];
                	i++;
                	break;
                default:
                    break;
                }
            }
            else
            {
                std::printf("Invalid argument: %s\n", argv[i]);
                return -1;
            }
        }
	}

	arc_path += "/arc.arc";
	arc_handle = gzopen(arc_path.c_str(), "rb");

	if(arc_handle == NULL)
	{
		std::printf("Unable to open a handle to arc.arc!\n");
		return -1;
	}

	// Read the entirety of arc.arc and populate all data structures
	generate_file_table(arc_handle, verbose, mft_dump);

	for(unsigned int i = 0; i < num_sections; i++)
	{
		dump_arc_section(i, out_path);
	}



	return 0;
}

