/** @file
 *  Definition of @ref angle.
 */
#ifndef SH3_ANGLE_HPP_INCLUDED
#define SH3_ANGLE_HPP_INCLUDED

#include <cmath>

#include <boost/math/constants/constants.hpp>
#include <boost/operators.hpp>

/**
 *  An angle.
 *  
 *  @tparam T The datatype for the internal representation.
 *  
 *  The angle is stored in radians.
 */
template<typename T>
struct angle final : public boost::additive<angle<T>>, public boost::less_than_comparable<angle<T>>, public boost::multiplicative<angle<T>, T>
{
public:
    constexpr angle() = default;
    constexpr angle(const angle&) = default;

    /**
     *  Create an 0-@ref angle.
     */
    static constexpr angle Zero() { return FromRadians(0); }
    /**
     *  Create an @ref angle representing a full turn.
     *  
     *  @note The returned @ref angle is not normalized, as this would just be equal to @ref Zero otherwise.
     */
    static constexpr angle Turn() { return FromRadians(boost::math::constants::two_pi<T>()); }

    /**
     *  Create an @ref angle from radians.
     *  
     *  @ref radians The angle in radians.
     */
    static constexpr angle FromRadians(T radians) { return angle{radians}; }
    /**
     *  Create an @ref angle from degrees.
     *  
     *  @ref degrees The angle in degrees.
     */
    static constexpr angle FromDegrees(T degrees) { return angle{degrees * boost::math::constants::degree<T>()}; }

    /**
     *  Get the numeric representation of the @ref angle in radians.
     */
    constexpr T AsRadians() const { return radians; }
    /**
     *  Get the numeric representation of the @ref angle in degrees.
     */
    constexpr T AsDegrees() const { return radians * boost::math::constants::radian<T>(); }

    constexpr bool operator<(const angle& other) const { return radians < other.radians; }
    constexpr bool IsExactlyZero() const { return std::fpclassify(radians) == FP_ZERO; }

    constexpr angle& operator=(const angle& other) = default;

    constexpr angle operator-() { return angle{-radians}; }
    constexpr angle& operator+=(const angle& other) { radians += other.radians; return *this; }
    constexpr angle& operator-=(const angle& other) { radians -= other.radians; return *this; }
    constexpr angle& operator*=(const T other) { radians *= other; return *this; }
    constexpr angle& operator/=(const T other) { radians /= other; return *this; }

    /**
     *  Normalize the internal representation.
     *  
     *  The internal representation is in radians, which is normalized to a number in (-pi, pi].
     */
    constexpr void Normalize() { *this = Normalized(); }
    /**
     *  Create a normalized @ref angle.
     *  
     *  @see @ref normalize
     *  
     *  @returns The normalized angle.
     */
    constexpr angle Normalized() const { return *this - Turn() * std::ceil((radians - boost::math::constants::pi<T>()) / boost::math::constants::two_pi<T>()); }

private:
    /**
     *  Construct an @ref angle.
     *  
     *  @param rad The angle in radians.
     */
    constexpr angle(T rad): radians{rad} {}

private:
    T radians;
};

using float_angle = angle<float>;
using double_angle = angle<double>;

//bugged in gcc < 7.2
#if (!defined(__GNUC__) || defined(__clang__)) \
 || __GNUC__ > 7 || (__GNUC__ == 7 && __GNUC_MINOR__ >= 2)
extern template struct angle<float>;
extern template struct angle<double>;
#endif

#endif
