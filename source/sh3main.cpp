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
#include <SH3/sh3math.hpp>
#include <SH3/arc/sh3_arc_types.hpp>
#include <SH3/system/sh3_window.hpp>
#include <SH3/system/sh3_config.hpp>
#include <iostream>

typedef struct
{
    float x, y, z, w;
} test;

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
    Log(LOG_INFO, "===SILENT HILL 3 REDUX===");
    Log(LOG_INFO, "Copyright 2016-2017 Palm Studios\n");

    test t1 = {1, 2, 3, 4};
    test t2 = {0, 2, 0, 0};
    test ret;

    ret = __sse_vector_muls(&t1, &t2);

    printf("%f", ret.y);

//    sh3_config config;
//    sh3_arc arc;
//
//    arc.Load();
//    arc.LoadFile("data/eff_tex/fire00_tr.pic", NULL);
//
//    sh3_window* window = new sh3_window(640, 480, "SILENT HILL 3");


    return 0;
}
