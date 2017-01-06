/** @file
 *  Assertion macros.
 *  
 *  @copyright 2017  Palm Studios
 */
#ifndef SH3_SYSTEM_ASSHERT_HPP_INCLUDED
#define SH3_SYSTEM_ASSHERT_HPP_INCLUDED

#ifndef DOXYGEN
#ifdef ASSERT_OFF
#define ASSERT_MSG_IMPL(cond, msg, ignore) static_cast<void>(0)
#else
#ifdef __GNUC__
#define ASSERT_FUNC __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define ASSERT_FUNC __FUNCSIG__
#else
#define ASSERT_FUNC __func__
#endif

#define ASSERT_MSG_IMPL(cond, msg, ignore) do {static bool assertIgnore = false; if(!assertIgnore && !(cond)) { if(ignore) assertIgnore = true; sh3_assert(assertIgnore, msg, __FILE__, __LINE__, ASSERT_FUNC); } } while(false)
#endif
#endif

/** @defgroup assert Assertion macros
 *  
 *  Check that conditions are held.
 *  
 *  If the condition does not hold, one of these actions will be taken:
 *    * default (nothing is defined): Log message and @c std::abort
 *    * @c ASSERT_OFF is defined: Nothing (default for release builds)
 *    * @c ASSERT_CONTINUE is defined: Log message (and continue)
 *    * @c ASSERT_ASK_MSGBOX is defined: Log message and show a graphical dialog-box asking how to continue
 *    * @c ASSERT_ASK_STDERR is defined: Log message and ask via STDIN how to continue
 *  
 *  @c ASSERT_ASK_MSGBOX and @c ASSERT_ASK_STDERR may both be defined, in which case the terminal will be tried if the dialog-box failed.
 *  Possible actions that can be taken are documented for @ref AskAction.
 *  
 *  @{
 */
/** @hideinitializer
 *  Debug-Assertion of a condition.
 *  
 *  @param cond The condition to assert.
 */
#define ASSERT(cond) ASSERT_MSG(cond, #cond)
/** @hideinitializer
 *  Debug-Assertion of a condition with failure-message.
 *  
 *  @see @ref ASSERT
 *  
 *  @param cond The condition to assert.
 *  @param msg  The message to show when the assertion failed.
 */
#define ASSERT_MSG(cond, msg) ASSERT_MSG_IMPL(cond, msg, false)
/** @hideinitializer
 *  Debug-Assertion of a condition once.
 *  
 *  @see @ref ASSERT
 *  
 *  @param cond The condition to assert.
 */
#define ASSERT_ONCE(cond) ASSERT_ONCE_MSG(cond, #cond)
/** @hideinitializer
 *  Debug-Assertion of a condition once with failure-message.
 *  
 *  @see @ref ASSERT_ONCE
 *  
 *  @param cond The condition to assert.
 *  @param msg  The message to show when the assertion failed.
 */
#define ASSERT_ONCE_MSG(cond, msg) ASSERT_MSG_IMPL(cond, msg, true)
/** @} */

/**
 *  The function that is invoked when an assertion failed.
 *  
 *  @see @ref ASSERT
 *  @see @ref ASSERT_MSG
 *  @see @ref ASSERT_ONCE
 *  @see @ref ASSERT_ONCE_MSG
 *  
 *  @param ignore Boolean indicating whether this assertion will be ignored next time.
 *  @param msg    Failure-message.
 *  @param file   Source code file in which the assertion takes place.
 *  @param line   Source code line in which the assertion takes place.
 *  @param func   Source code function in which the assertion takes place.
 */
void sh3_assert(bool &ignore, const char* msg, const char* file, int line, const char* func);

#endif //SH3_SYSTEM_ASSHERT_HPP_INCLUDED
