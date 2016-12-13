/*++

Copyright (c) 2016  Palm Studios

Module Name:
        stdtype.hpp

Abstract:
        Defines some common types, static functions and macros commonly used.

Author:
        Quaker762

Environment:

Notes:

Revision History:
        13-12-2016: Initial Revision [Quaker762]

--*/
#ifndef STDTYPE_HPP_INCLUDED
#define STDTYPE_HPP_INCLUDED

#include <cstdio>
#include <cstring>
#include <cstdarg>

/************************************************************/
/*                  STATIC FUNCTIONS                        */
/* These functions are commonly used throughout the project */
/* so they're implemented in a common header file           */
/*                                                          */
/************************************************************/

#define LOG_INFO    0
#define LOG_WARN    1
#define LOG_ERROR   2
#define LOG_FATAL   3

static inline void Log(int logType, char* str, ...)
{
    static int  firstOpen = 1;

    const char* filename = "log.txt";
    char        buff[4096];
    FILE*       logfile = NULL;
    va_list     args;
    unsigned int         res;

    if(firstOpen)
    {
        if((logfile = fopen(filename, "w+")) == NULL)
        {
            fprintf(stderr, "Unable to open a handle to %s", filename);
            return;
        }

        firstOpen = 0;
    }

    switch(logType)
    {
    case LOG_INFO:
        strcpy(buff, "[info]");
        break;

    case LOG_WARN:
        strcpy(buff, "[warning]");
        break;

    case LOG_ERROR:
        strcpy(buff, "[error]");
        break;

    case LOG_FATAL:
        strcpy(buff, "[fatal]");
        break;

    default:
        break;
    }

    if((logfile = fopen(filename, "a+")) == NULL) // Open the file to append, so we don't wipe the entire contents!
    {
        fprintf(stderr, "Unable to open a handle to %s", filename);
        return;
    }

    // Get our variable args, like %x, %s etcetc
    va_start(args, str);
    vsprintf(buff, str, args);
    va_end(args);

    if((res = fwrite(buff, 1, strlen(buff), logfile)) != strlen(buff))
    {
        fprintf(stderr, "Unable to write to flush info log!");
        return;
    }

    fclose(logfile);
}


/************************************************************/
/*                   TYPE DEFINITIONS                       */
/* Defines a few common types that are used throughout      */
/* the Engine                                               */
/************************************************************/

#define SHFALSE 0
#define SHTRUE  1

/**++
    These allow us to check more specifically for a bad allocation across multiple systems
    though the compiler/stdlib really should take care of it for us

    This is probably really bad programming, but it makes it a bit easier to read.

    (Is this only for programs with Debugging symbols enabled?!?!)
--**/

#ifdef _WIN32
    #define BADALLOC (void*)0xBAADF00D; // This is the null
#elif
    #define BADALLOC (void*)0; // Not sure what Linux/OSX define this as...
#endif

typedef int SHSTATUS;






























#endif // STDTYPE_HPP_INCLUDED
