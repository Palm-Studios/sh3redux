#include <iostream>
#include <utility>

#include <boost/container/flat_map.hpp>
#include <boost/range/adaptor/map.hpp>

#include <SDL.h>

#include "SH3/system/input.hpp"
#include "SH3/system/window.hpp"

using key_bindings = boost::container::flat_map<sh3::system::input_system::raw, sh3::system::input_system::action>;

namespace
{
    bool PollEvents(sh3::system::input_system &input, const key_bindings &bindings)
    {
        using sh3::system::input_system;

        SDL_PumpEvents();
        int mouseX, mouseY;
        SDL_GetRelativeMouseState(&mouseX, &mouseY);
        int wheelX = 0, wheelY = 0;

        input.StartUpdateActions();

        SDL_Event sdlEvent;
        while(SDL_PollEvent(&sdlEvent))
        {
            switch(sdlEvent.type)
            {
            case SDL_QUIT:
                return false;
            //TODO: mouse buttons
            case SDL_MOUSEWHEEL:
                wheelX += sdlEvent.wheel.x;
                wheelY += sdlEvent.wheel.y;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if(sdlEvent.key.repeat)
                {
                    break;
                }
                {
                    const auto binding = bindings.find(input_system::raw{sdlEvent.key.keysym.scancode});
                    if(binding == end(bindings))
                    {
                        break;
                    }
                    const auto state = sdlEvent.type == SDL_KEYDOWN ? input_system::state::PRESSED : sh3::system::input_system::state::RELEASED;
                    input.SetAction(binding->second, state, input_system::timestamp{sdlEvent.key.timestamp});
                }
                break;
            default:
                break;
            }
        }

        input.SetMovementDelta(vector2{static_cast<double>(mouseX), static_cast<double>(mouseY)}, vector2{static_cast<double>(wheelX), static_cast<double>(wheelY)});

        input.EndUpdateActions(input_system::action_state::sdl_clock::now());

        return true;
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    sh3_window window{800, 600, "input test"};
    sh3::system::input_system input;

    key_bindings bindings;
    bindings.emplace("W", "Forward");
    bindings.emplace("A", "Left");
    bindings.emplace("S", "Back");
    bindings.emplace("D", "Right");

    const auto actions = bindings | boost::adaptors::map_values;
    input.Prepare(begin(actions), end(actions));

    while(true)
    {
        if(!PollEvents(input, bindings))
        {
            break;
        }
        for(auto &action : actions)
        {
            using state = sh3::system::input_system::state;
            if(input[action].Was(state::PRESSED))
            {
                std::cout << action << " x " << input[action].Times(state::PRESSED) << ": " << input[action].NormalizedTime(state::PRESSED) << "; ";
            }
        }
        std::cout << std::endl;
        SDL_Delay(500);
    }

    return 0;
}
