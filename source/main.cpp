/** @file
 *  Program entry point.
 *
 *  @copyright 2006  Palm Studios
 *
 *  @author Jesse Buhagiar
 */

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
    // cppcheck-suppress noExplicitConstructor
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

/**
 *  Entry point to the program.
 *
 *  @note Requires @c argc and @c argv to make it compatible with SDL2.
 *
 *  @param argc Number of arguments.
 *  @param argv Argument vector.
 *
 *  @returns An @ref exit_code specifying success or failure.
 */
#if not defined(__GNUC__) || defined(__clang__)
[[noreturn]]
#endif
int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);
    Log(LogLevel::INFO, "===SILENT HILL 3 REDUX===");
    Log(LogLevel::INFO, "Copyright 2016-2017 Palm Studios\n");

    test t1 = {1, 2, 3, 4};
    test t2 = {0, 3, 0, 0};
    test ret;

    ret = t1 * t2;

    std::printf("%f\n", static_cast<double>(ret.y));

    ret = t1 + t2;

    std::printf("%f\n", static_cast<double>(ret.y));

//    sh3_config config;
//    sh3_arc arc;
//
//    arc.Load();
//    std::vector<uint8_t> data;
//    arc.LoadFile("data/eff_tex/fire00_tr.pic", data);
//
//    sh3_window* window = new sh3_window(640, 480, "SILENT HILL 3");

    exit(exit_code::SUCCESS);
}
