/** @file
 *
 *  Game state manager. Handles all requests to change, add and remove states from
 *  the current state list.
 *
 *  @copyright 2016-2019  Palm Studios
 *
 *  @date 10-2-2019
 *
 *  @author Jesse Buhagiar [quaker762]
 */

#ifndef _STATEMANAGER_HPP_
#define _STATEMANAGER_HPP_

#include "SH3/engine/gamestate.hpp"
#include "SH3/common/singleton.hpp"

#include <stack>
#include <memory>

namespace sh3 { namespace state {

/**
 *  State manager class. Provides us a way to manage states properly as well as providing an API
 *  to transition states (and have fancy effects).
 */
class CStateManager
{
public:
    /**
     *
     */
    CStateManager();

    /**
     *  dtor
     *
     *  'Unwinds' the stack
     */
    ~CStateManager();

    /**
     * Push a new state to the top of the stack and call @ref sh3::state::CGameState::Init()
     *
     * @param
     */
    void PushState(const CGameState& state);

    /**
     * Pop the top of the state stack and call @ref sh3::state::CGameState::Destroy()
     */
    void PopState(void);

    /**
     * Peek the top of the state stack
     *
     * @return Gamestate at the top of the stack, @ref stateStack
     */
    const std::unique_ptr<CGameState>& Peek(void) const;

private:
    std::stack<std::unique_ptr<CGameState>>     stateStack;  /**< State list. This could probably an std::vector too */
    std::uint64_t                               numStates;  /**< Number of states currently on the stack */
};

}}



#endif /* INCLUDE_SH3_ENGINE_STATEMANAGER_HPP_ */
