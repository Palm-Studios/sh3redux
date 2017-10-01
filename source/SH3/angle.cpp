#include "SH3/angle.hpp"

#if (!defined(__GNUC__) || defined(__clang__)) \
 || __GNUC__ > 7 || (__GNUC__ == 7 && __GNUC_MINOR__ >= 2)
template struct angle<float>;
template struct angle<double>;
#endif
