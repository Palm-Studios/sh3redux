/** @file
 *  Program entry point.
 *
 *  @copyright 2006  Palm Studios
 *
 *  @author Jesse Buhagiar
 */

#include "SH3/arc/types.hpp"
#include "SH3/system/config.hpp"
#include "SH3/system/exit_code.hpp"
#include "SH3/system/log.hpp"
#include "SH3/system/window.hpp"
#include "SH3/system/glprogram.hpp"
#include "SH3/graphics/msbmp.hpp"
#include "SH3/graphics/texture.hpp"
#include <cstdio>


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
int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    sh3_arc mft;
    mft.Load();

    Log(LogLevel::INFO, "===SILENT HILL 3 REDUX===");
    Log(LogLevel::INFO, "Copyright 2016-2017 Palm Studios\n");

    sh3_window window(640, 480, "sh3redux");

    sh3_graphics::msbmp("data/pic/error.bmp");

    sh3_graphics::sh3_texture tex(mft, "data/pic/sy/sys_warning.tex");

    return static_cast<int>(exit_code::SUCCESS);
}
