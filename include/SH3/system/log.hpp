/** @file
 *
 *  Defines logging functions.
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note
 *
 *  \date 1-1-2017
 *
 *  \author Alexander Hirsch
 */
#ifndef SH3_LOG_HPP_INCLUDED
#define SH3_LOG_HPP_INCLUDED

enum class LogLevel
{
    Info,
    Warn,
    Error,
    Fatal,
    None,
};

void Log(LogLevel logType, const char* str, ...);

void die(const char* str, ...);

#endif // SH3_LOG_HPP_INCLUDED
