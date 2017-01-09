/** @file
 *  Defines configuration variables for the @ref sh3::system::input_system.
 */
#ifndef SH3_SYSTEM_INPUT_CONFIG_HPP_INCLUDED
#define SH3_SYSTEM_INPUT_CONFIG_HPP_INCLUDED

/**
 *  Whether the @ref sh3::system::input_system should provide timing support for inputs.
 *  
 *  With this enabled, the @ref sh3::system::input_system keeps track what fraction of a frame the actions were active for.
 *  
 * @note Doesn't work right with SDL timestamps.
 *       I think the event times are not when the input *happened*, but when the event-loop *received* them, which is not useful to us.
 */
#ifdef DOXYGEN
#define INPUT_PROVIDE_TIMING 0
#else
#undef INPUT_PROVIDE_TIMING
#endif
/**
 *  Whether the @ref sh3::system::input_system should provide counting support for inputs.
 *  
 *  With this enabled, the @ref sh3::system::input_system keeps track how many times an action was triggered within a frame.
 */
#ifdef DOXYGEN
#define INPUT_PROVIDE_COUNT 1
#else
#define INPUT_PROVIDE_COUNT
#endif

#endif //SH3_SYSTEM_INPUT_CONFIG_HPP_INCLUDED
