/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3math.hpp

Abstract:
        Inline functions to speed up mathematical operations by using
        CPU specific features (like SSE)

Author:
        Quaker762

Environment:

Notes:

Revision History:
        17-12-2016: File Created [Quaker762]

--*/
#ifndef SH3MATH_HPP_INCLUDED
#define SH3MATH_HPP_INCLUDED

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
    __builtin_cpu_init(); // Tell GCC we want to check for an extension
    if(!__builtin_cpu_supports("sse3")) return -1;

    return 0;
}




/************************************************************/
/*               FLOATING-POINT OPERATIONS                  */
/*                                                          */
/************************************************************/



#endif // SH3MATH_HPP_INCLUDED
