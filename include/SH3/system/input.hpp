/** @file
 *  Defines the @ref sh3::system::input_system.
 */
#ifndef SH3_SYSTEM_INPUT_HPP_INCLUDED
#define SH3_SYSTEM_INPUT_HPP_INCLUDED

#include <bitset>
#include <chrono>
#include <cstdint>
#include <ratio>
#include <string>

#include <boost/container/flat_map.hpp>
#include <boost/operators.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <SDL_timer.h>

#include "input_config.hpp"

struct vector2
{
    double x, y;
};

namespace sh3 { namespace system {
    /**
     *  Status of various HIDs.
     *  
     *  Currently only keyboard and mouse.
     */
    struct input_system
    {
    public:
        /**
         *  The identifier for an action.
         *  
         *  An @ref action is usually bound to a key or a button.
         */
        // Using a fixed sized structure does not seem worth it.
        typedef std::string action;

        /**
         *  The state of an @ref action.
         */
        //TODO: better name?
        struct action_state final
        {
        public:
            /**
             *  Implements SDL Ticks as std TrivialClock concept
             */
            struct sdl_clock final
            {
            public:
                sdl_clock() = delete;

                //the sdl_clock should perhaps be somewhere else
                using duration = std::chrono::duration<Uint32, std::milli>;
                using rep =  duration::rep;
                using period = duration::period;
                struct time_point : public std::chrono::time_point<sdl_clock>
                {
                public:
                    /**
                     *  Constructor.
                     */
                    time_point() = default;

                    /**
                     *  Constructor.
                     *  
                     *  @param timestamp The SDL timestamp.
                     */
                    explicit time_point(const Uint32 timestamp) noexcept : time_point(duration(timestamp)) {}

                    using std::chrono::time_point<sdl_clock>::time_point;
                };

                static constexpr bool is_steady = true;

                static time_point now() noexcept { return time_point(SDL_GetTicks()); }
            };
            using timestamp = sdl_clock::time_point;
            using time_delta = sdl_clock::duration;

            /**
             *  State of an @ref action.
             */
            struct state final
            {
            public:
                /**
                 *  Partial @ref state representation.
                 *  
                 *  Used for public interface.
                 */
                enum simple : bool
                {
                    RELEASED = false, /**< @ref action is inactive */
                    PRESSED  = true   /**< @ref action is active */
                };

            private:
                enum index : std::size_t
                {
                    LAST_STATE,   /**< Last recorded @ref state. */
                    JUST_CHANGED, /**< @ref state changed this frame. */
                    //It does not seem useful to track this.
                    //TOGGLED,      /**< @ref state changed more than once this frame. */
                    LAST_INDEX    /**< Last @ref index. */
                };

                using status_set = std::bitset<index::LAST_INDEX>;

            public:
                /**
                 *  Checks whether this @ref state currently is in a given @ref simple state.
                 *  
                 *  @param other The @ref state to check against.
                 *  
                 *  @returns true if it is in that @ref simple state, false otherwise.
                 */
                bool Is(const simple other) const { return status[index::LAST_STATE] == other; }
                /**
                 *  Checks whether this @ref state has been in a given @ref simple state during
                 *  the last update.
                 *  
                 *  @param other The @ref state to check against.
                 *  
                 *  @returns true if it was in that @ref simple state, false otherwise.
                 */
                bool Was(const simple other) const { return status[index::JUST_CHANGED] || Is(other); }
                /**
                 *  Checks whether a this @ref state transitioned to a given @ref simple state during
                 *  the last update.
                 *  
                 *  @param other The @ref state to check against.
                 *  
                 *  @returns true if it became that @ref simple state, false otherwise.
                 */
                bool Became(const simple other) const { return status[index::JUST_CHANGED] && Is(other); }

                /**
                 *  Updates internal State.
                 *  
                 *  Should be called once in the game loop before
                 *  the event-loop for this tick starts.
                 */
                void Update() { status &= (1u << static_cast<unsigned>(index::LAST_STATE)); }

                /**
                 *  Set this state to a new state.
                 *  
                 *  @param other The new state.
                 */
                void Set(const simple other);

            private:
                status_set status;
            };

        public:
            /**
             *  Checks whether an @ref action is currently in a given @ref state.
             *  
             *  @param other The @ref state to check against.
             *  
             *  @returns true if it is in that @ref state, false otherwise.
             */
            bool Is(const state::simple other) const { return value.Is(other); }
            /**
             *  Checks whether an @ref action has been in a given @ref state during the last update.
             *  
             *  @param other The @ref state to check against.
             *  
             *  @returns true if it was in that @ref state, false otherwise.
             */
            bool Was(const state::simple other) const { return value.Was(other); }
            /**
             *  Checks whether an @ref action became a given @ref state during the last update.
             *  
             *  @param other The @ref state to check against.
             *  
             *  @returns true if it became that @ref state, false otherwise.
             */
            bool Became(const state::simple other) const { return value.Became(other); }

            /**
             *  Check how long the @ref action was in a @ref state this tick.
             *  
             *  @param other The @ref state to time.
             *  
             *  @returns The relative ratio [0..1].
             */
            float NormalizedTime(const state::simple other) const
            #ifdef INPUT_PROVIDE_TIMING
                { const auto &time = timing.relative; return other ? time.pressedTimeRatio : time.releasedTimeRatio; }
            #else
                { return Was(other); }
            #endif

            /**
             *  Check how many times the @ref action has entered a @ref state this tick.
             *  
             *  @param other Which @ref state to count.
             *  
             *  @returns The number of times the @ref state was entered.
             */
            std::size_t Times(const state::simple other) const
            #ifdef INPUT_PROVIDE_COUNT
                 { return count.halfNumChanges + Became(other); }
            #else
                 { return Became(other) || Was(other) && !Is(Other); }
            #endif

            /**
             *  Signal changing state of an @ref action.
             *  
             *  @param stamp The time the @ref action occurred.
             *  @param new_state The new state of the @ref action.
             */
            void Set(const timestamp stamp, const state::simple new_state);

            /**
             *  Updates internal @ref state.
             *  
             *  Should be called once in the game loop *before*
             *  the event-loop for this tick starts.
             */
            void StartUpdate();
            /**
             *  Updates internal @ref state.
             *  
             *  Should be called once in the game loop *after*
             *  the event-loop for this tick has ended.
             *  
             *  @param stamp The now.
             */
            void EndUpdate(const timestamp stamp);
            //FIXME: take delta time? feasible?

        private:
            state value;
            #ifdef INPUT_PROVIDE_TIMING
                timestamp lastTimestamp;
                //we only ever need either the absolute (during update) or relative timing (after update)
                union
                {
                    struct absoluteTimingInfo
                    {
                        time_delta pressedTime, releasedTime;
                    } absolute;
                    struct relativeTimingInfo
                    {
                        //FIXME: typedef
                        float pressedTimeRatio, releasedTimeRatio;
                    } relative;
                } timing;
            #endif
            #ifdef INPUT_PROVIDE_COUNT
                //we only ever need either the numChanges (during update) or half of it (after update)
                union
                {
                    //FIXME: typedef
                    std::size_t numChanges, halfNumChanges;
                } count;
            #endif
            //if DEBUG bool inUpdate; //check Timing and Count unions are used properly
        };

        //device id -> multiple mice/keyboards/joysticks/wheels/gamepads
        //          -> get rid of runtime-typed unions?
        enum axis
        {
            MOUSE_HORIZONTAL,
            MOUSE_VERTICAL,
            MOUSE_WHEEL,
            JOYSTICK_YAW,
            JOYSTICK_PITCH,
            JOYSTICK_ROLL,
            JOYSTICK_THRUST,
            WHEEL_ROLL,
            WHEEL_THRUST,
            WHEEL_BRAKE,
            GAMEPAD_RIGHT_HORIZONTAL,
            GAMEPAD_RIGHT_VERTICAl,
            GAMEPAD_LEFT_HORIZONTAL,
            GAMEPAD_LEFT_VERTICAL,
        };

        /*
        enum class mouseAxis
        {
            HORIZONTAL = MOUSE_HORIZONTAL,
            VERTICAL   = MOUSE_VERTICAL,
            WHEEL      = MOUSE_WHEEL
        };

        enum class joystickAxis
        {
            YAW    = JOYSTICK_YAW,
            PITCH  = JOYSTICK_PITCH,
            ROLL   = JOYSTICK_ROLL,
            THRUST = JOYSTICK_THRUST
        };

        enum class wheelAxis
        {
            ROLL   = WHEEL_ROLL,
            THRUST = WHEEL_THRUST,
            BRAKE  = WHEEL_BRAKE
        };

        enum class gamePadAxis
        {
            RIGHT_HORIZONTAL = GAMEPAD_RIGHT_HORIZONTAL,
            RIGHT_VERTICAL = GAMEPAD_RIGHT_VERTICAL,
            LEFT_HORIZONTAL = GAMEPAD_LEFT_HORIZONTAL,
            LEFT_VERTICAL = GAMEPAD_LEFT_VERTICAL
        };
        */

        /**
         *  "Raw" (untranslated) input.
         *  For example a keycode or mouse button index.
         */
        struct raw final : public boost::totally_ordered<raw>
        {
        private:
            enum class type : std::uint8_t
            {
                KEYBOARD_KEY,
                MOUSE_BUTTON,
                //AXIS_THRESHOLD_REACHED,
                //JOYSTICK_BUTTON,
                //JOYSTICK_HAT,
                //GAMEPAD_BUTTON,
                //GAMEPAD_HAT,
            };

        public:
            /**
             *  Constuctor.
             *  
             *  @param name The literal name for the raw input.
             */
            raw(const std::string &name);
            //TODO: explicit?
            //static from KeyboardScancode?
            constexpr raw(const SDL_Scancode scancode): rawType(type::KEYBOARD_KEY), keyboardKey(scancode) {}

            /**
             *  Equality check.
             *  
             *  @param other The input to check against.
             *  
             *  @returns Whether or not this and the other input refer to the same.
             */
            bool operator==(const raw &other) const;
            /**
             *  Smaller-than check.
             *  
             *  @param other The input to check against.
             *  
             *  @returns Whether or not this input is ordered before the other.
             */
            bool operator<(const raw &other) const;

        private:
            type rawType;
            union
            {
                SDL_Scancode keyboardKey;
                decltype(SDL_GetMouseState(nullptr, nullptr)) mouseButton;
                //size_t wheelButton;
                //int jostickButton;
                //int gamepadButton;
            };
        };

        using timestamp = action_state::timestamp;
        using state = action_state::state::simple;

    private:
        typedef boost::container::flat_map<action, action_state> action_state_map;

    public:
        /**
         *  Return the current @ref action_state of an @ref action.
         *  
         *  @param the_action The @ref action to retrieve the @ref action_state for.
         */
        const action_state& operator[](const action &the_action) const;
        /**
         *  Return the current @ref action_state of an @ref action.
         *  
         *  @param the_action The @ref action to retrieve the @ref action_state for.
         */
        action_state& operator[](const action &the_action);

        /**
         *  Returns the mouse movement relative to the last frames position.
         *  The X and Y components represent cursor movement in the horizontal and vertical axis.
         *  
         *  @see GetWheelScrollDelta
         */
        //TODO: more generic. GetAxisDelta(index)
        const vector2& GetMouseMovementDelta() const { return mouseMovementDelta; }

        /**
         *  Returns the mouse wheel scroll done this frame.
         *  The X and Y components represent wheel movement in the horizontal and vertical axis.
         *  
         *  @see GetMouseMovementDelta
         */
        const vector2& GetWheelScrollDelta() const { return wheelScrollDelta; }
        //TODO: absolute positions

        /**
         *  Reserves memory for at least size entries in the internal @ref action_state_map.
         *  
         *  @param size The number of entries to reserve memory for.
         */
        void Reserve(const action_state_map::size_type size) { actionStates.reserve(size); }

        /**
         *  Resets the @ref actionStates.
         *  Does not allocated free memory.
         */
        void Clear() { actionStates.clear(); }

        /**
         *  Prepare the @ref input_system to receive actions.
         *  
         *  @tparam iterator The iterator-type for the parameters.
         *  
         *  @param begin An begin-iterator to something convertible to @ref action.
         *  @param end   An end-iterator to something convertible to @ref action.
         *  
         */
        template<typename iterator>
        void Prepare(const iterator begin, const iterator end)
        {
            auto entryRange = boost::make_iterator_range(begin, end)
                            | boost::adaptors::transformed([](const action &action){ return std::make_pair(action, action_state()); });
            actionStates.insert(entryRange.begin(), entryRange.end());
        }

        /**
         *  Signal that an @ref action was toggled.
         *  Sets the @ref action in the @ref action_state_map to the new @ref action_state::state.
         *  
         *  @param action    The @ref action to set.
         *  @param new_state The new @ref action_state::state of the @ref action.
         *  @param stamp     The time the action occurred.
         */
        void SetAction(const action &action, const state new_state, const timestamp stamp) { (*this)[action].Set(stamp, new_state); }

        /**
         *  Updates @ref actionStates for the next frame.
         *  Should be called before translating input to @ref SetAction.
         *  
         *  @see EndUpdateActions
         */
        void StartUpdateActions() { for(auto &state : actionStates | boost::adaptors::map_values) { state.StartUpdate(); } }
        /**
         *  Updates @ref actionStates for the previous frame.
         *  Should be called after translating input to @ref SetAction.
         *  
         *  @see StartUpdateActions
         */
        void EndUpdateActions(const timestamp stamp) { for(auto &state : actionStates | boost::adaptors::map_values) { state.EndUpdate(stamp); } }

        //TODO: more generic. SetAxisDelta(index, delta)
        template<typename vec2>
        void SetMovementDelta(vec2 &&mouseDelta, vec2 &&wheelDelta) { mouseMovementDelta = std::forward<vec2>(mouseDelta); wheelScrollDelta = std::forward<vec2>(wheelDelta); }

    private:
        action_state& GetState(const action &action);

    private:
        action_state_map actionStates;

        vector2 mouseMovementDelta, wheelScrollDelta;
    };

    /**
     *  Negate the @ref sh3::system::input_system::action_state::state::simple.
     *  
     *  @param state the @ref sh3::system::input_system::action_state::state::simple to negate.
     *  
     *  @returns The negated @ref sh3::system::input_system::action_state::state::simple.
     */
    inline input_system::action_state::state::simple operator!(const input_system::action_state::state::simple state)
    {
        return static_cast<input_system::action_state::state::simple>(!static_cast<bool>(state));
    }

    inline void input_system::action_state::state::Set(const simple state)
    {
        assert(status[index::LAST_STATE] != state);

        status[index::LAST_STATE] = state;

        /*
        if(status[index::JUST_CHANGED])
        {
            status[index::TOGGLED] = true;
        }
        */

        status[index::JUST_CHANGED] = true;
    }
    inline auto input_system::operator[](const action &action) const -> const action_state&
    {
        const auto iter = actionStates.find(action);
        assert(iter != actionStates.cend());
        return iter->second;
    }

    inline auto input_system::operator[](const action &action) -> action_state&
    {
        const auto iter = actionStates.find(action);
        assert(iter != actionStates.cend());
        return iter->second;
    }
} }

#endif //SH3_SYSTEM_INPUT_HPP_INCLUDED
