/** @file
 *  Defines logging functions.
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 22-12-2016
 *
 *  @author Alexander Hirsch
 */
#ifndef SH3_LOG_HPP_INCLUDED
#define SH3_LOG_HPP_INCLUDED

// windows.h may define this
#ifdef ERROR
#undef ERROR
#endif

/**
 *  A level of error to tell the user how serious a message is.
 *
 *      INFO    - Information tag, general information to give to the user.
 *      WARN    - Warning tag, tell the user something naughty may have happened.
 *      ERROR   - Error tag, tell the user something bad has probably happened.
 *      FATAL   - Fatal tag, tell the user something REALLY bad has happened, and the program probably terminated early.
 *      NONE    - No tag to append to message.
 */
enum class LogLevel
{
    INFO,
    WARN,
    ERROR,
    FATAL,
    NONE,
};

/**
 *  Write a string to the log file.
 *
 *  @param str - Formatted string to print.
 *
 *  @return Nothing
 */
[[gnu::format(printf, 2, 3)]] void Log(LogLevel logType, const char* str, ...);

/**
 *  Kill the process due to a fatal error being encountered
 *  and inform the user of the problem (as well as writing to
 *  the error log, log.txt)
 *
 *  @param str - Formatted string to print.
 *
 *  @return Nothing (@c noreturn)
 */
[[noreturn]] [[gnu::format(printf, 1, 2)]] void die(const char* str, ...);

#endif // SH3_LOG_HPP_INCLUDED
