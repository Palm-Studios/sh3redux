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

#include <algorithm>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

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
    static const char* CFGPATH = "sh3r.cfg";

    std::fstream cfgfile(CFGPATH);
    if(!cfgfile)
    {
        Log(LogLevel::ERROR, "Unable to find %s! Reverting to default values...", CFGPATH);
        return -1;
    }

    int         nStrs = 0;
    std::string command;

    while(getline(cfgfile, command))
    {
        static const std::string COMMENT = { '#', '$' };
        if(command.empty() || COMMENT.find(command.front()) != std::string::npos)
        {
            continue;
        }

        // Now, split the command up and store its' value in our map (that we can use later!! :] )
        const auto commandEnd = find(begin(command), end(command), ' ');
        std::string key(begin(command), commandEnd),
                    value(commandEnd, end(command));
        values[std::move(key)] = std::stoi(std::move(value)); // <- THIS IS VERY DANGEROUS!!!!!!!!

        nStrs++;
    }

    return nStrs;
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
