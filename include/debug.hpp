/*++

Copyright (c) 2016  Palm Studios

Module Name:

Abstract:

Author:

Environment:

Notes:

Revision History:
        28-11-2016: Initial Revision

--*/
#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED

#define LEVEL_INFO  0
#define LEVEL_WARN  1
#define LEVEL_ERROR 2

extern void debug_print(int level, const char* dstr);


#endif // DEBUG_HPP_INCLUDED
