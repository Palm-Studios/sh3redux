/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_config.cpp

Abstract:
        Implementation of sh3_config.hpp

Author:
        Quaker762

Environment:

Notes:

Revision History:
        27-12-2016: File Created                                                    [Quaker762]

--*/
#include <SH3/system/sh3_config.hpp>
#include <SH3/stdtype.hpp>
#include <cstring>
#include <cstdio>

const char* CFGPATH = "sh3r.cfg";
const int   commandSize = 256;

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
    FILE*   cfgfile;

    if((cfgfile = fopen(CFGPATH, "r")) != NULL)
    {
        int         ptr;
        int         nStrs;
        char        commandc[512];
        std::string command;

        while(!feof(cfgfile))
        {
            fgets(commandc, 512, cfgfile); // Get in one command of text

            command = commandc;

            // Now, split the command up and store its' value in our map (that we can use later!! :] )
            if(command.c_str()[0] == '#' || command.c_str()[0] == '$') // This is a comment, ignore it!
            {
                continue;
            }
            else
            {
                char* tokens;





                nStrs++;
            }
        }

        return nStrs;
    }
    else
    {
        Log(LOG_ERROR, "Unable to find %s! Reverting to default values...", CFGPATH);
        return -1;
    }
}

