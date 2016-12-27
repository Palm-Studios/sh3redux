/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_log.cpp

Abstract:
        Implementation of the Log( ) function so we don't have any issues with static variables
        across translation units!

Author:
        Quaker762

Environment:

Notes:

Revision History:
        24-12-2016: File Created                                                    [Quaker762]
                    Function copied and implemented
                    Added a new type, LOG_NONE

--*/
#define LOG_INFO    0
#define LOG_WARN    1
#define LOG_ERROR   2
#define LOG_FATAL   3
#define LOG_NONE    4

#include <cstdio>
#include <cstring>
#include <cstdarg>

void Log(int logType, const char* str, ...)
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
    case LOG_NONE: // Lmao
        break;

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

    fprintf(logfile, "\n"); // Add a new line!
    fclose(logfile);
}
