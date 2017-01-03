/** @file
 *  Definition of exit codes.
 *  
 *  @copyright 2016  Palm Studios
 */

#ifndef SH3_SYSTEM_EXIT_CODE_HPP_INCLUDED
#define SH3_SYSTEM_EXIT_CODE_HPP_INCLUDED

#include <cstdint>
#include <cstdlib>

/**
 *  Exit codes.
 *  
 *  Return these from @ref main() or pass to @c std::exit() & co.
 *  
 *  It's a @c std::uint8_t to be compatible with systems that only use 8-bit exit statuses.
 *  
 *  @c Success must remain 0.
 *  126 and 127 are used by shells by convention and should thus not be used.
 */
enum class exit_code : std::uint8_t
{
    SUCCESS,  /**< everything went fine, apparently */
    DEATH,    /**< exit from @ref die() */
};
static_assert(static_cast<int>(exit_code::SUCCESS) == 0, "must remain 0 to indicate success");

/**
 *  Call @c std::exit with the @ref exit_code.
 *  
 *   <c>enum class</c> has no implicit cast to @c int, so this is a convenience function.
 *   
 *   @param code The code to exit with.
 */
[[noreturn]] static inline int exit(exit_code code)
{
    std::exit(static_cast<int>(code));
}

#endif // SH3_SYSTEM_EXIT_CODE_HPP_INCLUDED
