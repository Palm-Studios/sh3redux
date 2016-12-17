/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3arch.hpp

Abstract:
        CPU Architecture checking functions etc

Author:
        Quaker762

Environment:

Notes:
        Possible CPU names are as follows:

        ‘intel’
            Intel CPU.
        ‘atom’
            Intel Atom CPU.
        ‘core2’
            Intel Core 2 CPU.
        ‘corei7’
            Intel Core i7 CPU.
        ‘nehalem’
            Intel Core i7 Nehalem CPU.
        ‘westmere’
            Intel Core i7 Westmere CPU.
        ‘sandybridge’
            Intel Core i7 Sandy Bridge CPU.
        ‘amd’
            AMD CPU.
        ‘amdfam10h’
            AMD Family 10h CPU.
        ‘barcelona’
            AMD Family 10h Barcelona CPU.
        ‘shanghai’
            AMD Family 10h Shanghai CPU.
        ‘istanbul’
            AMD Family 10h Istanbul CPU.
        ‘btver1’
            AMD Family 14h CPU.
        ‘amdfam15h’
            AMD Family 15h CPU.
        ‘bdver1’
            AMD Family 15h Bulldozer version 1.
        ‘bdver2’
            AMD Family 15h Bulldozer version 2.
        ‘bdver3’
            AMD Family 15h Bulldozer version 3.
        ‘btver2’
            AMD Family 16h CPU.

Revision History:
        17-12-2016: File Created [Quaker762]

--*/
#ifndef SH3ARCH_HPP_INCLUDED
#define SH3ARCH_HPP_INCLUDED

/*++

Routine Description:
        Check what CPU the user is running

Arguments:
        cpu - CPU name

Return Type:
        0 - User has CPU of name/type cpu
       -1 - User is using an unknown/unsupported arch

--*/
static inline int __check_cpu(const char* cpu)
{
    __builtin_cpu_init(); // Tell GCC we want to check for a CPU related item
    if(!__builtin_cpu_is(cpu)) return -1;

    return 0;
}



#endif // SH3ARCH_HPP_INCLUDED
