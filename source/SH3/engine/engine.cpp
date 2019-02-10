/** @file
 *
 *  Implementation of engine.hpp.
 *
 *  Control is then passed to the main game loop.
 *
 *  @copyright 2016-2019 Palm Studios
 *
 *  @date 9-2-2019
 *
 *  @author Jesse Buhagiar [quaker762]
 */
#include "SH3/engine/engine.hpp"


#include <iostream>
#include <thread>

using namespace sh3::engine;
using namespace std::chrono;

CEngine::CEngine()
    : config(), running(false)
{

}

CEngine::~CEngine()
{

}

void CEngine::Init(const std::string& args)
{
    running = true;
    Run();
}

void CEngine::Run(void) noexcept
{
    std::uint64_t now;          // Current time
    std::uint64_t frameTime;    // The time it took to render one frame
    std::uint64_t accum;        // Time accumlulator
    std::uint64_t lastTime = 0; // The value of 'now' for currentFrame - 1
    std::uint64_t frames = 0;   // Frame counter. NOT frames per second!

    while(running)
    {
        now = clock.GetTimeMilliseconds();
        frameTime = now - lastTime;
        accum += frameTime;
        frames++;

        /**
         *  Seeing as we're locked to 60fps, if the previous frame time was
         *  less than our target frame time, we'll just sleep for the difference
         *  to ensure that we still run at 60FPS :)
         */
        if(frameTime < FRAME_TIME_TARGET)
        {
            std::uint64_t diff = FRAME_TIME_TARGET - frameTime;
            std::this_thread::sleep_for(std::chrono::milliseconds(diff));
        }

        // input
        // update
        // render

        // Do some shit
        if(accum >= clock.SECOND_IN_MS)
        {
            std::printf("frames rendered: %llu, last frametime: %llu\n", frames, frameTime);
            accum = 0;
            frames = 0;
        }

        lastTime = now;
    }
}
