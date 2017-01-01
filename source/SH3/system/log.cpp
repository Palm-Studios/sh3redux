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

#include "SH3/system/log.hpp"
#include "SH3/stdtype.hpp"

#include <cstdio>
#include <cstring>
#include <cstdarg>

#include <SDL2/SDL_messagebox.h>

void Log(LogLevel logType, const char* str, ...)
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

    const char* label = "";
    switch(logType)
    {
    case LogLevel::Info:
        label = "[info] ";
        break;

    case LogLevel::Warn:
        label = "[warning] ";
        break;

    case LogLevel::Error:
        label = "[error] ";
        break;

    case LogLevel::Fatal:
        label = "[fatal] ";
        break;

    case LogLevel::None:
        break;
    }

    va_start(args, str);
    if(fputs(label, logfile) < 0 || vfprintf(logfile, str, args) < 0)
    {
        fprintf(stderr, "Unable to write to flush info log!");
    }
    else
    {
        fputc('\n', logfile);
    }
    va_end(args);
}

void die(const char* str, ...)
{
    va_list args;
    char    buff[4096];

    va_start(args, str);
    vsnprintf(buff, sizeof(buff), str, args);
    Log(LogLevel::Fatal, buff);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", buff, nullptr);
    va_end(args);
    exit(SH_BAD);
}
