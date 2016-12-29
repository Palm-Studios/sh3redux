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
template<typename T> static T __sse_vector_add(const T* v1, const T* v2)
{
    T ret; // This is pushed onto the stack first (ESP+12)

    #ifdef SH3_64 // 64-bit build
    __asm__
    (
        ""
    );
    #else // 32-bit build
    __asm__
    (
        "MOV EAX, [ESP+12]\n"        // EAX contains pointer to v1
        "MOV EBX, [ESP+16]\n"        // EBX contains pointer to v2
        "MOVUPS XMM0, [EAX]\n"
        "MOVUPS XMM1, [EBX]\n"
        "MULPS XMM0, XMM1\n"
        "MOVUPS [ESP+8], XMM0"
    );
    #endif

    return ret;
}

/*++

Routine Description:
        Multiply a vector by a scalar using SSE3 Registers and operations

Arguments:
        T& v1 - Reference to the first vector/vertex
        flt x - Scalar

Return Type:
        typename T

--*/
template<typename T> static T __sse_vector_muls(const T* v1, const T* v2)
{
    T ret; // This is pushed onto the stack first (ESP+12)

    #ifdef SH3_64 // 64-bit build
    __asm__
    (
        ""
    );
    #else // 32-bit build
    __asm__
    (
        "MOV EAX, [ESP+12]\n"        // EAX contains pointer to v1
        "MOV EBX, [ESP+16]\n"        // EBX contains pointer to v2
        "MOV ECX, [ESP+8]\n"
        "MOVUPS XMM0, [EAX]\n"
        "MOVUPS XMM1, [EBX]\n"
        "MULPS XMM0, XMM1\n"
        "MOVUPS [ECX], XMM0"
    );
    #endif

    return ret;
}

#endif // SH3MATH_HPP_INCLUDED
