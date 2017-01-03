/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3log.hpp

Abstract:
        Defines logging functions.

Author:
        Alexander Hirsch

Environment:

Notes:

Revision History:

--*/
#ifndef SH3_LOG_HPP_INCLUDED
#define SH3_LOG_HPP_INCLUDED

enum class LogLevel
{
    INFO,
    WARN,
    ERROR,
    FATAL,
    NONE,
};

void Log(LogLevel logType, const char* str, ...);

/*++

Routine Description:
        Kill the process due to a fatal error being encountered
        and inform the user of the problem (as well as writing to
        the error log, log.txt)

Arguments:
        str - String to display to the user

Return Type:
        None

--*/
void die(const char* str, ...);

#endif // SH3_LOG_HPP_INCLUDED
