/** @file
 *
 *  Implementation of sh3_config.hpp
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note
 *
 *  \date 27-12-2016
 *
 *  \author Jesse Buhagiar
 */
#include "SH3/system/config.hpp"
#include "SH3/stdtype.hpp"

#include <sstream>
#include <cstring>
#include <cstdio>
#include <vector>

#include <iostream>

/** Default configuration path */
const char* CFGPATH = "sh3r.cfg";
const int   commandSize = 256;

/**
 *  Load all options and map them to their values in our map
 *
 *  @param
 *
 *  @returns \c int - Number of Strings read in from the file
*/
int sh3_config::Load()
{
    std::FILE* cfgfile;

    if((cfgfile = std::fopen(CFGPATH, "r")) != NULL)
    {
        int         nStrs;
        char        commandc[512];
        std::string command;

        std::vector<std::string> args;

        while(!std::feof(cfgfile))
        {
            std::fgets(commandc, 512, cfgfile); // Get in one command of text
            command = commandc;

            // Now, split the command up and store its' value in our map (that we can use later!! :] )
            if(command.c_str()[0] == '#' || command.c_str()[0] == '$' || command.c_str()[0] == '\n' || command.c_str()[0] == '\0') // This is a comment, ignore it!
            {
                continue;
            }
            else
            {
                std::stringstream iss(command);

                for(std::string s; iss >> s;) // Split the string
                    args.push_back(s);

                values[args[0]] = std::stoi(args[1]); // <- THIS IS VERY DANGEROUS!!!!!!!!
                args.clear();

                nStrs++;
            }
        }

        return nStrs;
    }
    else
    {
        Log(LogLevel::Error, "Unable to find %s! Reverting to default values...", CFGPATH);
        return -1;
    }
}

/**
 *  Kill the process due to a fatal error being encountered
 *  and inform the user of the problem (as well as writing to
 *  the error log, log.txt)
 *
 *  @param str - String to display to the user
 *
 *  @returns Nothing
*/
int sh3_config::GetOptionValue(std::string option)
{
    if(values.find(option) == values.end())
        return -1;

    return values[option];
}



