/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3main.cpp

Abstract:
        Program entry point.

Author:
        Quaker762

Environment:

Notes:

Revision History:
        13-12-2016: Initial Revision [Quaker762]

--*/

#include <SH3/stdtype.hpp>
#include <iostream>

/*++

Routine Description:
        Entry point to the program.
        Requires argc and argv to make it compatible with SDL2

Arguments:
        argc - Number of arguments
        argv - Arguments list

Return Type:
        SHSTATUS

--*/
SHSTATUS main(int argc, char** argv)
{
    std::cout << "Hello World!";

    return SHTRUE;
}
