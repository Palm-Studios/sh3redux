/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_config.cpp

Abstract:
        Implementation of sh3_config.hpp

Author:
        Jesse Buhagiar

Environment:

Notes:

Revision History:
        27-12-2016: File Created                                                    [jbuhagiar]

--*/
#include "SH3/system/config.hpp"
#include "SH3/system/log.hpp"

#include <sstream>
#include <cstring>
#include <cstdio>
#include <vector>

#include <iostream>

static const char*   CFGPATH = "sh3r.cfg";

/*++

Routine Description:
        Load all options and map them to their values in our map

Arguments:
        None

Return Type:
        int - Number of Strings read in from the file

--*/
int sh3_config::Load()
{
    std::FILE* cfgfile;

    if((cfgfile = std::fopen(CFGPATH, "r")) != NULL)
    {
        int         nStrs = 0;
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
        Log(LogLevel::ERROR, "Unable to find %s! Reverting to default values...", CFGPATH);
        return -1;
    }
}

/*++

Routine Description:


Arguments:


Return Type:


--*/
int sh3_config::GetOptionValue(const std::string& option)
{
    auto iter = values.find(option);
    if(iter == end(values))
        return -1;

    return iter->second;
}
