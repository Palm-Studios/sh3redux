/** @file
 *  Window class and related functions
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 22-12-2016
 *
 *  Revision History:
 *      24-3-2019:
 *          + Complete class rewrite.
 *          + Move class into correct namespace (remove of sh3_ "C style" namespace)
 *
 *  @author Jesse Buhagiar
 */
#ifndef SH3_WINDOW_HPP_INCLUDED
#define SH3_WINDOW_HPP_INCLUDED

#include <memory>
#include <string>
#include <SDL_video.h>
#include "SH3/system/glcontext.hpp"

namespace sh3 { namespace system {

/**
 *  Describes a logical Window to interface with SDL2
 */
class CWindow
{
public:
    static constexpr int SCREEN_WIDTH_DEFAULT   = 1280;     /**< Default screen width(for graphical "safe mode") */
    static constexpr int SCREEN_HEIGHT_DEFAULT  = 1024;     /**< Default screen height (for graphical "safe mode") */

public:
    /**
     * Class constructor
     *
     * @param width     Window width
     * @param height    Window height
     * @param title     Window title (for windowed mode)
     */
    CWindow(int width, int height, const std::string& title);

    /**
     * Get the handle to the physical SDL2 window pointer
     *
     * @return Returns a <i>physical</i> pointer to @ref hwnd, the non-smart version of the
     * the SDL2 window.
     */
    const SDL_Window* GetHandle(void) const noexcept;

    /**
     *  Set the size of the window
     *
     *  width   The width we want to set the window to
     *  height  The height we want to set the window to
     */
    void SetSize(int width, int height);

    /**
     * Destroy this window
     */
    void Destroy(void);

    /**
     * Query whether or not this window is fullscreen.
     *
     * @return The value of @ref fullscreen
     */
    bool IsFullScreen(void) const {return fullscreen;}

private:
    bool                                        fullscreen;     /**< Is this window currently fullscreen? */
    std::string                                 title;          /**< Window title (for windowed mode) */

    std::unique_ptr<SDL_Window, sdl_destroyer>  hwnd;           /**< Our window handle */
    sh3::system::CRenderContext                 context;        /**< This window's OpenGL Context */
    std::vector<SDL_DisplayMode*>               displayModes;   /**< Display modes list */
};

}}

#endif // SH3_WINDOW_HPP_INCLUDED
