/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3main.cpp

Abstract:
        Program entry point.

Author:
        Jesse Buhagiar

Environment:

Notes:

Revision History:
        13-12-2016: Initial Revision [jbuhagiar]

--*/

#include "SH3/math.hpp"
#include "SH3/arc/types.hpp"
#include "SH3/system/config.hpp"
#include "SH3/system/exit_code.hpp"
#include "SH3/system/log.hpp"
#include "SH3/system/window.hpp"
#include <cstdio>

struct test
{
    friend test operator*(const test& v1, const test& v2);
    friend test operator+(const test& v1, const test& v2);

private:
    using vector = float __attribute__((vector_size(16)));
    const vector& as_vec() const { return reinterpret_cast<const vector&>(*this); }
    test(const vector& v): test(v[0], v[1], v[2], v[3]) {}

public:
    test(): test(0, 0, 0, 0) {}
    test(float a, float b, float c, float d): x(a), y(b), z(c), w(d) {}

    float x, y, z, w;
};

test operator*(const test& v1, const test& v2)
{
    return v1.as_vec() * v2.as_vec();
}

test operator+(const test& v1, const test& v2)
{
    return v1.as_vec() + v2.as_vec();
}

/*++

Routine Description:
        Entry point to the program.
        Requires argc and argv to make it compatible with SDL2

Arguments:
        argc - Number of arguments
        argv - Arguments list

Return Type:
        int

--*/
int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);
    Log(LogLevel::Info, "===SILENT HILL 3 REDUX===");
    Log(LogLevel::Info, "Copyright 2016-2017 Palm Studios\n");

    test t1 = {1, 2, 3, 4};
    test t2 = {0, 3, 0, 0};
    test ret;

    ret = t1 * t2;

    std::printf("%f\n", ret.y);

    ret = t1 + t2;

    std::printf("%f\n", ret.y);

//    sh3_config config;
//    sh3_arc arc;
//
//    arc.Load();
//    std::vector<uint8_t> data;
//    arc.LoadFile("data/eff_tex/fire00_tr.pic", data);
//
//    sh3_window* window = new sh3_window(640, 480, "SILENT HILL 3");


    exit(exit_code::Success);
}
