/** @file
 *
 *  C++ High resolution timer wrapper class
 *
 *  @copyright 2016-2019  Palm Studios
 *
 *  @date 10-2-2019
 *
 *  @author Jesse Buhagiar [quaker762]
 */

#ifndef _CLOCK_HPP_
#define _CLOCK_HPP_

#include <chrono>
#include <ctime>
#include <ratio>
#include <cstdint>

namespace sh3 { namespace system {

/**
 * Clock structure.
 *
 * Wrapper for the high resolution chronograph that is part of libstdc++
 */
struct clock_t final
{

    static constexpr std::uint64_t SECOND_IN_MS = 1000;
    static constexpr std::uint64_t SECOND_IN_NS = 1000000000;

public:
    /**
     * Get the current timestamp in milliseconds
     *
     * @return Returns the current millisecond timestamp from @c std::chrono::high_resolution_clock::now()
     */
    const std::uint64_t GetTimeMilliseconds(void) const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    /**
     * Get the current timestamp in nanoseconds
     *
     * @return Returns the current nanosecond timestamp from @c std::chrono::high_resolution_clock::now()
     */
    const std::uint64_t GetTimeNanoseconds(void) const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

private:
};

}}






#endif /* INCLUDE_SH3_SYSTEM_CLOCK_HPP_ */
