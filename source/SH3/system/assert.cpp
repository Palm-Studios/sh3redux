/** @file
 *  Assertion macros helpers.
 *  
 *  @copyright 2017  Palm Studios
 */
#ifndef ASSERT_OFF

#include "SH3/system/assert.hpp"

#include <iostream>
#include <locale>
#include <limits>
#include <string>

#include "SH3/system/log.hpp"

#ifndef ASSERT_CONTINUE
#include <cstdlib>
#endif

#if defined(DOXYGEN) || defined(ASSERT_ASK_MSGBOX) || defined(ASSERT_ASK_STDERR)
#define ASSERT_ASK 1

#include <debugbreak.h>

// we use SDL's datastructure regardless
#include <SDL_messagebox.h>

/**
 *  Possible actions a user can take when assertions fail.
 */
enum AskAction : int
{
    ABORT,     /**< Abort the program via @c std::abort. */
    BREAK,     /**< Issue a debug break. The program will likely just fail if no debugger is attached. */
    CONTINUE,  /**< Continue execution. */
    IGNORE,    /**< Continue execution and ignore future failures of this assertion. */
};

/**
 *  Show a messagebox asking how to continue.
 *  
 *  If @c ASSERT_ASK_MSGBOX is not set, simply return @c false.
 *  
 *  Uses standard I/O to present the available @ref AskAction "AskActions" to continue.
 *  
 *  @tparam nAskActions The number of buttons (should be automatically deduced).
 *  
 *  @param[out] selected The selected action.
 *  @param      message  Message do display on the dialog.
 *  @param      buttons  Buttons showing the available actions.
 *  
 *  @returns true if the user was queried successfully, false otherwise.
 */
template<std::size_t nAskActions>
static bool ask_msgbox(AskAction& selected, const std::string& message, const std::array<SDL_MessageBoxButtonData, nAskActions>& buttons)
{
    #ifndef ASSERT_ASK_MSGBOX
    static_cast<void>(selected);
    static_cast<void>(message);
    static_cast<void>(buttons);
    return false;
    #else
    SDL_MessageBoxData box = {
        SDL_MESSAGEBOX_ERROR,
        nullptr, // FIXME: get parent window
        "Assertion failure",
        message.c_str(),
        static_cast<int>(buttons.size()),
        buttons.data(),
        nullptr
    };
    return SDL_ShowMessageBox(&box, reinterpret_cast<int*>(&selected)) == 0;
    #endif
}

/**
 *  Ask via standard I/O how to continue.
 *  
 *  If @c ASSERT_ASK_STDERR is not set, simply return @c false.
 *  
 *  Uses standard I/O to present the available @ref AskAction "AskActions" to continue.
 *  
 *  @tparam nAskActions The number of buttons (should be automatically deduced).
 *  
 *  @param[out] selected The selected action.
 *  @param      buttons  Buttons showing the available actions.
 *  
 *  @returns @c true if the user was queried successfully, @c false otherwise.
 */
template<std::size_t nAskActions>
static bool ask_stderr(AskAction& selected, const std::array<SDL_MessageBoxButtonData, nAskActions>& buttons)
{
    #ifndef ASSERT_ASK_STDERR
    static_cast<void>(selected);
    static_cast<void>(buttons);
    return false;
    #else
    std::cerr << "How to proceed?" << std::endl;
    while(true)
    {
        std::cerr << "Select one of the options by entering one of the characters in the [square brackets]." << std::endl;
        for(const SDL_MessageBoxButtonData& button: buttons)
        {
            //TODO: check if this create two options with the same key
            std::cerr << '[' << button.text[0] << ']' << (button.text + 1) << ", ";
        }
        std::cerr << "\b\b? " << std::flush;
        char choice = static_cast<char>(std::tolower(std::cin.get()));
        if(choice == '\n')
        {
            //nothing
        }
        else if(std::cin.get() == '\n')
        {
            for(const SDL_MessageBoxButtonData& button: buttons)
            {
                if(choice == std::tolower(button.text[0]))
                {
                    selected = AskAction(button.buttonid);
                    return true;
                }
            }
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cerr << "Invalid choice." << std::endl;
    }
    #endif
}
#endif

/**
 *  Ask the user how to continue.
 *  
 *  Ask the user how to continue and takes the selected action.
 *  
 *  The possible actions are documented in @ref AskAction.
 *  
 *  @param[in,out] ignore  Boolean indicating whether this assertion will be ignored next time.
 *  @param         message Assertion message.
 *  
 *  @see @ref ask_msgbox
 *  @see @ref ask_stderr
 *  
 *  @returns @c true if an action was taken, @c false otherwise (user could not be queried).
 */
static bool ask(bool& ignore, const std::string& message)
{
    #ifndef ASSERT_ASK
    static_cast<void>(ignore);
    static_cast<void>(message);
    return false;
    #else
    const char* ignore_text = ignore ? "Ignore Off" : "Ignore On";
    std::array<SDL_MessageBoxButtonData, 4> buttons{{
        {0, AskAction::ABORT   , "Abort"    },
        {0, AskAction::BREAK   , "Break"    },
        {0, AskAction::CONTINUE, "Continue" },
        {0, AskAction::IGNORE  , ignore_text}
    }};
    AskAction selected;
    if(!ask_msgbox(selected, message, buttons))
    {
        if(!ask_stderr(selected, buttons))
        {
            std::cerr << "Could not make interactive assertion proceedings decision." << std::endl;
            return false;
        }
    }

    switch(selected)
    {
    case AskAction::ABORT:
        std::abort();
    case AskAction::BREAK:
        debug_break();
        break;
    case AskAction::IGNORE:
        ignore = !ignore;
        break;
    case AskAction::CONTINUE:
        break;
    }
    return true;
    #endif
}

void sh3_assert(bool &ignore, const char* msg, const char* file, int line, const char* func)
{
    static const std::string ignoreMessage("Assertion ignored from now on.");
    static const std::string unignoreMessage("Assertion not ignored anymore.");

    std::string fullMessage = std::string("Assertion failed in function ") + func + " at " + file + ":" + std::to_string(line) + ": " + msg + ".";

    if(ignore)
    {
        fullMessage += "\n" + ignoreMessage;
    }

    std::cerr << fullMessage << std::endl;
    Log(LogLevel::FATAL, "%s", fullMessage.c_str());

    bool preIgnore = ignore;
    if(ask(ignore, fullMessage))
    {
        if(ignore != preIgnore)
        {
            const std::string& message = ignore ? ignoreMessage : unignoreMessage;
            std::cerr << message << std::endl;
            Log(LogLevel::INFO, "%s", message.c_str());
        }
    }
    else
    {
        #ifndef ASSERT_CONTINUE
        std::abort();
        #endif
    }
}

#endif
