/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3math.hpp

Abstract:
        Inline functions to speed up mathematical operations by using
        CPU specific features (like SSE)

Author:
        Jesse Buhagiar

Environment:

Notes:
    http://neilkemp.us/src/sse_tutorial/sse_tutorial.html

Revision History:
        17-12-2016: File Created [jbuhagiar]

--*/
#ifndef SH3MATH_HPP_INCLUDED
#define SH3MATH_HPP_INCLUDED

#include <iostream>

/*++

Routine Description:
       Check if sse3 is available on the processor

Arguments:
        None

Return Type:
        0 - Supported
       -1 - Unsupported
--*/

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
