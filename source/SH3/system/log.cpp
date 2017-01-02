/** @file
 *
 *  Implementation of the Log( ) function so we don't have any issues with static variables
 *  across translation units!
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note
 *
 *  \date 24-12-2016
 *
 *  \author Jesse Buhagiar
 */
#include "SH3/system/log.hpp"
#include "SH3/stdtype.hpp"

#include <cstdio>
#include <cstdarg>
#include <SDL2/SDL_messagebox.h>

#include <SDL2/SDL_messagebox.h>

/**
 *  Write a message to the information log
 *
 *  @param LogLevel logType - Level of this warning
 *  @param const char* str - Message and arguments
 *
 *  @returns Nothing
*/
void Log(LogLevel logType, const char* str, ...)
{
    static const char* filename = "log.txt";
    static std::FILE*  logfile  = nullptr;

    std::va_list args;

    if(!logfile)
    {
        if(!(logfile = std::fopen(filename, "w+")))
        {
            std::fprintf(stderr, "Unable to open a handle to %s", filename);
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
    if(std::fputs(label, logfile) < 0 || std::vfprintf(logfile, str, args) < 0)
    {
        std::fprintf(stderr, "Unable to write to flush info log!");
    }
    else
    {
        std::fputc('\n', logfile);
    }
    va_end(args);
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
void die(const char* str, ...)
{
    std::va_list args;
    char         buff[4096];

    va_start(args, str);
    std::vsnprintf(buff, sizeof(buff), str, args);
    Log(LogLevel::Fatal, buff);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", buff, nullptr);
    va_end(args);
    exit(SH_BAD);
}
