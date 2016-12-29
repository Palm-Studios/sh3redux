/*++

Copyright (c) 2016  Palm Studios

Module Name:
        stdtype.hpp

Abstract:
        Defines some common types, static functions and macros commonly used.

        All other headers/source files should include this header!

Author:
        Quaker762

Environment:

Notes:

Revision History:
        13-12-2016: Initial Revision [Quaker762]
        14-12-2016: Added messagebox( ) function [Quaker762]
        17-12-2016: Added safedel( ) template function [Quaker762]

--*/
#ifndef STDTYPE_HPP_INCLUDED
#define STDTYPE_HPP_INCLUDED

#include <cstdio>
#include <cstring>
#include <cstdarg>

    // Cross platform ;)
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif __APPLE__

#elif __linux__

#endif

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
#define LOG_NONE    4

/*
    We need this here to avoid the stupid overwriting bug we get (well, not really
    a bug because each file gets its own copy of the static function) so we stick an
    external defintion here and implement in another file!! :^]
*/
extern void Log(int logType, const char* str, ...);

static inline void messagebox(const char* title, const char* str, ...)
{
    char        buff[4096];
    va_list     args;

    // Get our variable args, like %x, %s etcetc
    va_start(args, str);
    vsprintf(buff, str, args);
    va_end(args);

    // Cross platform ;)
    #ifdef _WIN32
        MessageBox(NULL, buff, title, MB_OK);
    #elif __APPLE__ // No idea for this, Apple probably have it behine 602348483468034684056 layers of fucking bullshit, the arseholes.

    #elif __linux__ // This no way in hell this will be standard, we should probably just have the console open!

    #endif
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
    #define BADALLOC (void*)0xBAADF00D; // This is the nullptr on Windows Systems (I think...)
#else
    #define BADALLOC (void*)0; // Not sure what Linux/OSX define this as...
#endif

typedef int SHSTATUS;












/************************************************************/
/*                  TEMPLATE FUNCTIONS                      */
/*                                                          */
/************************************************************/

// Template to make sure if we delete, we aren't deleting
// a nullptr (because segfaults are baddddd)
template <class T> void safedelete(T* p)
{
    if(p != nullptr)
    {
        delete p;
        p = nullptr;
    }
}

// Same as above, but for arrays!
template <class T> void safedelete_arr(T* p)
{
    if(p != nullptr)
    {
        delete[] p;
        p = nullptr;
    }
}





























#endif // STDTYPE_HPP_INCLUDED
