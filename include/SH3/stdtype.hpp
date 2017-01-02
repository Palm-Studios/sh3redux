/** @file
 *
 *  Defines some common types, static functions and macros commonly used.
 *
 *  All other headers/source files should include this header!
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note
 *
 *  \date 13-12-2016
 *
 *  \author Jesse Buhagiar
 */
#ifndef STDTYPE_HPP_INCLUDED
#define STDTYPE_HPP_INCLUDED

#include <cstdio>
#include <cstring>
#include <cstdarg>

#include "SH3/system/log.hpp"

    // Cross platform ;)
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif __APPLE__

#elif __linux__

#endif

/************************************************************/
/*                   TYPE DEFINITIONS                       */
/* Defines a few common types that are used throughout      */
/* the Engine                                               */
/************************************************************/

#define SHFALSE 0
#define SHTRUE  1

#define SH_OK   0
#define SH_BAD  -1

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

typedef int SHSTATUS; // Return type for functions

#endif // STDTYPE_HPP_INCLUDED
