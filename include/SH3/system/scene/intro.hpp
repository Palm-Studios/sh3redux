/** @file
 *
 *  Scene that displays the Konami logo, Palm Studios logo, warning etc. The intro video (op.000)
 *  is not played in this scene.
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 5-1-2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef INTRO_H_INCLUDED
#define INTRO_H_INCLUDED

#include "SH3/system/scene/scene.hpp"

#include <cstdint>

namespace sh3_state
{
    class sh3_scene_intro : public sh3_scene
    {
    public:

        sh3_scene_intro() : timeElapsed(0){}

        /**
         *  Draw all logos located in /data/pic.
         *
         *  @param dt The time it took for the last frame to finish.
         */
        void Draw(float dt) override;

        /**
         *  Update the time variable and check whether we need to change scenes.
         *
         *  @param dt The time it took for the last frame to finish.
         */
        void Update(float dt) override;

    private:
        std::uint64_t timeElapsed;
    };
}


#endif // INTRO_H_INCLUDED
