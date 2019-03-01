/** @file
 *
 *  Engine class definition. This is the main instance of the game running that resides in memory.
 *  When @c main() is entered by the system, this class is instantiated, and control is then transferred to
 *  the init function. This is handles allocating all resources before the game is fully running etc.
 *
 *  Control is then passed to the main game loop.
 *
 *  @copyright 2016-2019 Palm Studios
 *
 *  @date 9-2-2019
 *
 *  @author Jesse Buhagiar [quaker762]
 */
#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include "SH3/common/singleton.hpp"
#include "SH3/system/config.hpp"
#include "SH3/system/clock.hpp"
#include "SH3/engine/statemanager.hpp"

#include <string>
#include <cstdint>
#include <chrono>

namespace sh3 { namespace engine {

/**
 *
 */
class CEngine : public CSingleton<CEngine>
{
public:
    static constexpr std::uint32_t FRAMES_PER_SECOND    = 60;   /**< Number of FPS the game is expected to run at (max) */
    static constexpr std::uint32_t FRAME_TIME_TARGET    = 1000 / FRAMES_PER_SECOND; /**< Target frame time, which is around 16.66ms */

public:
    /**
     *
     */
    CEngine();


    ~CEngine();

    /**
     * Engine initialisation functions
     *
     * @param args Arguments passed to the engine.
     */
    void Init(const std::string& args);

private:

    /**
     * Main engine loop. Controls engine anf framerate control/timing.
     *
     * This loop is very simple, as it merely caps the framerate to a silky smooth
     * 60FPS. Obviously on older hardware this will result in the game running slower,
     * HOWEVER, considering the power of hardware has increased nearly 10000000x since
     * SILENT HILL 3 was released, this will really not be an issue.
     */
    void Run(void) noexcept;

private:
    sh3_config                  config;         /**< The engine's configuration file */
    bool                        running;        /**< Is the game currently running? */
    sh3::system::clock_t        clock;          /**< Game clock (for loop timing)*/
    sh3::state::CStateManager   stateManager;
};

}}







#endif /* INCLUDE_SH3_ENGINE_ENGINE_HPP_ */
