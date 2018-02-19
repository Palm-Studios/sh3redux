/**
 *  Prints out ALL data of a specified file (or list of file) in all data types.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdint>
#include <cassert>

#include "SH3/arc/mft.hpp"
#include "SH3/arc/vfile.hpp"

std::ofstream ofile; // Horrible scoping, I know...

void f_info(sh3::arc::mft& mft, const std::string& fname, bool verbose)
{    
	sh3::arc::vfile             cur_file; // Current file we are analysing.
    std::vector<std::uint32_t>  data;
    sh3::arc::vfile::read_error e;
    char                        line[1024];
    std::size_t                 fsize;

    cur_file.Open(mft, fname);
    fsize = cur_file.GetFilesize();
    data.resize(fsize / 4);
    cur_file.ReadData(&data[0], fsize / 4, e);

    std::printf("Reading file: %s...\n", fname.c_str());
    std::sprintf(line, "---===[D A T A    A N A L Y S I S : %s]===---\n", fname.c_str());
    ofile << line;  
    if(verbose)
    {
        std::printf("%s", line);
    }

    for(unsigned int i = 0; i < data.size(); i++)
    {
        std::uint32_t   dword;
        std::uint16_t   word[2];
        std::uint8_t    byte[4];
        float           f;
        char            txt[5];

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        dword   = data[i];
        
        word[0] = static_cast<uint16_t>(dword >> 16);
        word[1] = static_cast<uint16_t>(dword & 0xffff);

        byte[0] = static_cast<std::uint8_t>(dword >> 24);
        byte[1] = static_cast<std::uint8_t>(dword >> 16);
        byte[2] = static_cast<std::uint8_t>(dword >> 8);
        byte[3] = static_cast<std::uint8_t>(dword & 0xff);

        f       = static_cast<float>(dword);
        
        // I couldn't be fucked doing the math for this, so it's going to stay like this!
        txt[0] = static_cast<char>((dword >> 24) & 0xFF);
        txt[1] = static_cast<char>((dword >> 16) & 0xFF);
        txt[2] = static_cast<char>((dword >> 8) & 0xFF);
        txt[3] = static_cast<char>(dword & 0xFF);         

        for(int j = 0; j < 4; j++)
        {
            if(txt[j] < 0x20)
            {
                txt[j] = '.';
            }
        }
        
        std::sprintf(line, "0x%08x - [0x%x] \t[%.8f] \t[%d] \t[%d %d] \t[%d %d %d %d] [%c %c %c %c] -- \n", i * 4, dword, f, dword, word[0], word[1], byte[0], byte[1], byte[2], byte[3], txt[0], txt[1], txt[2], txt[3]);
        
        if(verbose)
        {
           std:: printf("%s", line);
        }

        ofile << line;
    }
}

int main(int argc, char** argv)
{
    std::string     of_name; // Output file name
    std::string     flist; // File list path (if required)
    std::string     if_name; // Input file name
    sh3::arc::mft   mft;
    bool            list = false;
    bool            verbose = false; // Should we print information to the terminal??
    
    of_name = "output.txt";

    if(argc == 1)
    {
        std::printf("sh3r File Info Dumper. Copyright 2016-2018 Palm Studios\n\n");
        std::printf("No input file detected!\n\n");
        std::printf("Usage:\t-v: Print all file info to the terminal as well as output file\n");
        std::printf("      \t-i: Input file list to get information from\n");
        std::printf("      \t-o: Output file to write file information to. Default is %s\n", of_name.c_str());
        std::printf("      \t-f: Open single file\n");
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
                case 'v':
                    verbose = true;
                    break;
                case 'i':
                    flist = argv[i + 1];
                    list = true;
                    std::printf("flist: %s\n", flist.c_str());
                    i++;
                    break;
                case 'o':
                    of_name = argv[i + 1];
                    std::printf("of_name: %s\n", of_name.c_str());
                    i++;
                    break;
                case 'f':
                    if_name = argv[i + 1];
                    std::printf("if_name: %s\n", if_name.c_str());
                    list = false;
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

    ofile.open(of_name, std::ios::trunc);

    if(list)
    {
        std::ifstream flist_stream;
        std::string tmp;
        std::vector<std::string> filenames;

        std::printf("Reading file list...\n");
        flist_stream.open(flist);
        while(std::getline(flist_stream, tmp))
        {
            filenames.push_back(tmp);
        }

        std::printf("Done!\n");
        for(unsigned int i = 0; i < filenames.size(); i++)
        {
            f_info(mft, filenames[i], verbose);
        }
    }
    else
    {
    	f_info(mft, if_name, verbose);
    }

    ofile.close();    

    return 0;
}
