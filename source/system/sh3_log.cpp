/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_log.cpp

Abstract:
        Implementation of the Log( ) function so we don't have any issues with static variables
        across translation units!

Author:
        Jesse Buhagiar

Environment:

Notes:

Revision History:
        24-12-2016: File Created                                                    [jbuhagiar]
                    Function copied and implemented
                    Added a new type, LOG_NONE

--*/

#include "SH3/stdtype.hpp"

#include <cstdio>
#include <cstring>
#include <cstdarg>

void Log(int logType, const char* str, ...)
{
    static const char* filename = "log.txt";
    static FILE*       logfile  = nullptr;

    va_list args;

    if(!logfile)
    {
        if(!(logfile = fopen(filename, "w+")))
        {
            fprintf(stderr, "Unable to open a handle to %s", filename);
            // fallback to stderr then
            logfile = stderr;
        }
    }

    switch(logType)
    {
    case LOG_INFO:
        fputs("[info] ", logfile);
        break;

    case LOG_WARN:
        fputs("[warning] ", logfile);
        break;

    case LOG_ERROR:
        fputs("[error] ", logfile);
        break;

    case LOG_FATAL:
        fputs("[fatal] ", logfile);
        break;

    case LOG_NONE: // Lmao
    default:
        break;
    }

    va_start(args, str);
    if(vfprintf(logfile, str, args) < 0)
    {
        fprintf(stderr, "Unable to write to flush info log!");
    }
    else
    {
        fputc('\n', logfile);
    }
    va_end(args);
}
