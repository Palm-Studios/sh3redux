/** @file
 *  Inline functions to speed up mathematical operations by using CPU specific features (like SSE)
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 27-12-2016
 *
 *  @note See http://neilkemp.us/src/sse_tutorial/sse_tutorial.html
 *
 *  @author Jesse Buhagiar
 */
#ifndef SH3MATH_HPP_INCLUDED
#define SH3MATH_HPP_INCLUDED

#include <iostream>

/**
 *  Check if sse3 is available on the processor
 *
 *  @param cpu - CPU name
 *
 *  @return 0 or -1, depending on whether sse3 is supported.
 */
static inline int __check_sse3()
{
    #ifdef __GNUC__
    #ifndef __clang__
    // Tell GCC we want to check for an extension
    // clang does not support thing, but it's also not needed.
    __builtin_cpu_init();
    #endif
    if(!__builtin_cpu_supports("sse3")) return -1;
    #else
    #error Cannot check CPU extensions with this compiler.
    #endif

    return 0;
}

#endif // SH3MATH_HPP_INCLUDED
