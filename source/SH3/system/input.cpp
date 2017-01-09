#include "SH3/system/input.hpp"

#include <cstdint>
#include <string>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/type_traits/extent.hpp>

#include <SDL2/SDL_keyboard.h>

#include <SH3/system/assert.hpp>
#include <SH3/system/log.hpp>
#include "SH3/system/input_config.hpp"

using namespace sh3::system;

void input_system::action_state::StartUpdate()
{
    #ifdef INPUT_PROVIDE_TIMING
        timing.absolute.pressedTime = timing.absolute.releasedTime = time_delta::zero();
    #endif
    #ifdef INPUT_PROVIDE_COUNT
        count.numChanges = 0;
    #endif

    value.Update();
}

void input_system::action_state::EndUpdate(const timestamp stamp)
{
    #ifndef INPUT_PROVIDE_TIMING
        static_cast<void>(stamp);
    #else
        if(Is(state::PRESSED))
        {
            timing.absolute.pressedTime = stamp - lastTimestamp;
        }
        else
        {
            timing.absolute.releasedTime = stamp - lastTimestamp;
        }

        const time_delta deltaTime = timing.absolute.pressedTime + timing.absolute.releasedTime;
        if(deltaTime == time_delta::zero())
        {
            timing.relative.pressedTimeRatio  = 0.5;
            timing.relative.releasedTimeRatio = 0.5;
        }
        else
        {
            //note: we have to cache these locally, because they are in an union
            const auto pressedTime  = timing.absolute.pressedTime.count();
            const auto releasedTime = timing.absolute.releasedTime.count();
            timing.relative.pressedTimeRatio  = static_cast<float>(pressedTime  / deltaTime.count());
            timing.relative.releasedTimeRatio = static_cast<float>(releasedTime / deltaTime.count());
        }

        lastTimestamp = stamp;
    #endif

    #ifdef INPUT_PROVIDE_COUNT
        count.halfNumChanges = count.numChanges / 2;
    #endif
}

void input_system::action_state::Set(const timestamp stamp, const state::simple new_state)
{
    #ifndef INPUT_PROVIDE_TIMING
        static_cast<void>(stamp);
    #else
        timing.absolute.releasedTime += stamp - lastTimestamp;
        lastTimestamp = stamp;
    #endif
    #ifdef INPUT_PROVIDE_COUNT
        ++count.numChanges;
    #endif

    value.Set(new_state);
}

input_system::raw::raw(const std::string &name)
{
    constexpr auto mouseButtonString = "Mouse Button ";
    if(boost::algorithm::starts_with(name, mouseButtonString))
    {
        try
        {
            std::size_t button = std::stoul(name.substr(boost::extent<decltype(mouseButtonString)>::value));
            rawType = type::MOUSE_BUTTON;
            mouseButton = static_cast<decltype(mouseButton)>(SDL_BUTTON(button));
            return;
        }
        catch(std::logic_error&)
        {
        }
    }

    rawType = type::KEYBOARD_KEY;
    keyboardKey = SDL_GetScancodeFromName(name.c_str());

    if(keyboardKey == SDL_SCANCODE_UNKNOWN)
    {
        Log(LogLevel::WARN, "Unknown input \"%s\".", name.c_str());
    }
}

bool input_system::raw::operator==(const raw &other) const
{
    if(rawType != other.rawType)
    {
        return false;
    }

    switch(rawType)
    {
    case type::KEYBOARD_KEY:
        return keyboardKey == other.keyboardKey;
    case type::MOUSE_BUTTON:
        return mouseButton == other.mouseButton;
    default:
        ASSERT(false);
        return false;
    }
}

bool input_system::raw::operator<(const raw &other) const
{
    if(rawType != other.rawType)
    {
        return rawType < other.rawType;
    }

    switch(rawType)
    {
    case type::KEYBOARD_KEY:
        return keyboardKey < other.keyboardKey;
    case type::MOUSE_BUTTON:
        return mouseButton < other.mouseButton;
    default:
        ASSERT(false);
        return false;
    }
}
