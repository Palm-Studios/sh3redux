/** @file
 *
 *  Game State abstract class. Each 'screen' in the game (such as the main menu, the inventory menu
 *  etc) can be modelled as a 'game state'. Each state has control over it's own drawing and updating.
 *  This way, we can seperate each portion of the game into its' own little module.
 *
 *  <i>ALL</i> game states states must inherit from this class.
 *
 *  @copyright 2016-2019  Palm Studios
 *
 *  @date 10-2-2019
 *
 *  @author Jesse Buhagiar [quaker762]
 */

#ifndef _GAMESTATE_HPP_
#define _GAMESTATE_HPP_

#include "SH3/engine/statemanager.hpp"
#include "SH3/arc/mft.hpp"

#include <string>
#include <memory>

#include <SDL.h>

namespace sh3 { namespace state {

class CStateManager; // Forward declaration to prevent circular inclusion

/**
 * Game state base class. This class is pure virtual.
 */
class CGameState
{
public:

    /**
     * Default Constructor
     *
     * @warning This constructor is deleted
     */
    CGameState() = delete;

    /**
     * Class constructor
     *
     * Passes reference of the game state manager to this state. If this is not the case,
     * it is <i>impossible</i> for this state to transition to another!
     */
    CGameState(CStateManager& mgr)
    : name(""), id(0), stateManager(mgr)
    {

    }

    CGameState(const CGameState& rhs)
        : name(rhs.name), id(rhs.id), stateManager(rhs.stateManager)
        {}

    /**
     *  Virtual Destructor
     */
    virtual ~CGameState(){}

    /**
     * State intiailisation function. When the state manager makes a call to
     * "changeState", the previous states 'destroy' function is called (unless
     * we specify that it should NOT be destroyed) and the init function is run.
     *
     *
     */
    virtual void Init(void) noexcept = 0;

    /**
     *  Cleanup
     *
     *  Most of this could be in the dtor, however it makes more sense for us to have full
     *  control of when we want cleanup to occur.
     */
    virtual void Destroy(void) noexcept = 0;

    /**
     * State update function.
     *
     * The 'heartbeat' of this game state
     */
    virtual void Update(void) noexcept = 0;

    /**
     * State render function.
     *
     * All rendering (OpenGL) stuff is to be put in this function
     */
    virtual void Render(void) noexcept = 0;

    /**
     * Input handler function.
     *
     * All input is handled/passed here (@z33ky should have a better idea of how this works)
     */
    virtual void InputHandler(const SDL_Event& event) noexcept = 0;

    virtual std::unique_ptr<CGameState> clone() const = 0;

protected:
    std::string     name;               /**< The name of this state */
    std::uint64_t   id;                 /**< Numerical ID for this state */
    CStateManager&  stateManager;       /**< Reference to @ref sh3::engine::CEngine::stateManager */
    sh3::arc::mft   mft;
};

}}






#endif /* INCLUDE_SH3_ENGINE_GAMESTATE_HPP_ */
