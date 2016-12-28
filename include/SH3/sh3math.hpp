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
    http://neilkemp.us/src/sse_tutorial/sse_tutorial.html

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

/*++

Routine Description:
        Add two vectors using SSE3 Registers and operations

Arguments:
        T& v1 - Reference to the first vector/vertex
        T& v2 - Reference to second vector/vertex

Return Type:
        typename T

--*/
template<typename T> static T __sse_vector_add(const T& v1, const T& v2)
{
    T ret = {0, 0, 0, 0}; // This is pushed onto the stack first (ESP+12)

    __asm__
    (
        "MOV EBP, ESP\n"
        "MOV EAX, [EBP+4]\n"               // Store pointer to v1 in EBX
        "MOV EBX, [EBP+8]\n"               // Store pointer to v2 in EBX
        "MOVUPS XMM0, [EAX]\n"
        "MOVUPS XMM1, [EBX]\n"
        "ADDPS XMM0, XMM1\n"
        "MOVUPS [ESP+12], XMM0\n"
    );

    return ret;
}

static int __test_asm(int x)
{
    int y = 0;

    __asm__
    (
        "movw EAX, [EBP+8]\n"   // x is here
        "movw [ESP+12], EAX"    // y is here
    );

    return y;
}

#endif // SH3MATH_HPP_INCLUDED
