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
#include "SH3/graphics/msbmp.hpp"
#include "SH3/engine/state/intro.hpp"

#include <iostream>
#include <thread>


using namespace sh3::engine;
using namespace std::chrono;

CEngine::CEngine()
    : config(), running(false), hwnd(640, 480, "SILENT HILL 3: Redux")
{

}

CEngine::~CEngine()
{

}

void CEngine::Init(const std::string& args)
{
    static_cast<void>(args);

    // Initialise the window/video subsystem
    hwnd.Create(1280, 1024, "SILENT HILL 3: Redux");
    // States
    sh3::state::CIntroState intro(stateManager);

    stateManager.PushState(intro);

    running = true;
    Run();
}

void CEngine::Run(void) noexcept
{
    bool            first = true;
    std::uint64_t   now;          // Current time
    std::uint64_t   frameTime;    // The time it took to render one frame
    std::uint64_t   elapsedTime;  // Time accumlulator
    std::uint64_t   lastTime = 0; // The value of 'now' for currentFrame - 1
    std::uint64_t   frames = 0;   // Frame counter. NOT frames per second!
    std::uint64_t   diff = 0;

    while(running)
    {
        now = clock.GetTimeMilliseconds();
        elapsedTime += frameTime;

        if(first)
        {
            first = false;
            lastTime = now;
            continue;
        }

        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_QUIT)
                running = false;
        }

        stateManager.Peek().get()->InputHandler(event);
        stateManager.Peek().get()->Update();
        stateManager.Peek().get()->Render();
        SDL_GL_SwapWindow(const_cast<SDL_Window*>(hwnd.GetHandle()));

        if(elapsedTime >= clock.SECOND_IN_MS)
        {
            std::printf("frames rendered: %llu, last frametime: %llu\n", frames, frameTime);
            elapsedTime = 0;
            frames = 0;
        }

        /**
         *  Seeing as we're locked to 60fps, if the previous frame time was
         *  less than our target frame time, we'll just sleep for the difference
         *  to ensure that we still run at 60FPS :)
         *
         *  This doesn't work (yet!)
         */
//        if(frameTime < FRAME_TIME_TARGET)
//        {
//            std::uint64_t sleepTime = now + FRAME_TIME_TARGET - clock.GetTimeMilliseconds();
//            std::this_thread::sleep_for(std::chrono::milliseconds(now + FRAME_TIME_TARGET - clock.GetTimeMilliseconds()));
//        }

        frameTime = clock.GetTimeMilliseconds() - lastTime;
        frames++;
        lastTime = now;
        std::this_thread::sleep_for(std::chrono::milliseconds(now + FRAME_TIME_TARGET - clock.GetTimeMilliseconds()));
    }
}
