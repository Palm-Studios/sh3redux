/**
 *	Wrapper program for sh3::arc::vfile::Dump2Disk()
 *
 *	@date 20-2-2018
 *
 *	@author Jesse Buhagiar
 *
 *  @copyright 2016-2017  Palm Studios
 */
#include <iostream>
#include <cstdio>

#include <SH3/arc/mft.hpp>
#include <SH3/arc/vfile.hpp>

int main(int argc, char** argv)
{
	sh3::arc::mft 	mft;
	sh3::arc::vfile file;
	std::string		fname;

	if(argc == 1)
	{
        std::printf("sh3r File Dump Utility. Copyright 2016-2018 Palm Studios\n\n");
        std::printf("No filename detected!\n\n");
        std::printf("Usage: \t-f [filename]: Input filename to dump from arc section.");
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
                case 'f':
                	fname = argv[i+1];
                	i++;
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

	file.Open(mft, fname);
	file.Dump2Disk();

	return 0;
}
