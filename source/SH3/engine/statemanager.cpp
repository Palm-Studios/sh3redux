/** @file
 *
 *  Implementation of statemanager.cpp
 *
 *  @copyright 2016-2019  Palm Studios
 *
 *  @date 10-2-2019
 *
 *  @author Jesse Buhagiar [quaker762]
 */
#include "SH3/engine/statemanager.hpp"
#include "SH3/system/log.hpp"

#include <iostream>

#include <SDL.h>

using namespace sh3::state;

class StackProtector : public CGameState
{
public:

    StackProtector(CStateManager& mgr) : CGameState(mgr)
    {
        name = "GUARD_STATE";
        id = 0;
    }


    /**
     *
     */
    ~StackProtector()
    {
    }

    virtual std::unique_ptr<CGameState> clone() const override
    {
        return std::make_unique<StackProtector>(*this);
    }

    virtual void Init(void) noexcept
    {
        // Do nothing
    }

    virtual void Destroy(void) noexcept
    {
    }

    virtual void Update(void) noexcept
    {
        std::cout << "TICK ";
    }

    virtual void Render(void) noexcept
    {
        std::cout << "TOCK" << std::endl;
    }

    virtual void InputHandler(const SDL_Event& event) noexcept
    {
        // Do nothing
    }
};

CStateManager::CStateManager(void)
    : stateStack(), numStates(0)
{
    StackProtector protect(*this);

    PushState(protect);
}

CStateManager::~CStateManager(void)
{
    // Unwind the stack
    for(std::size_t i = 0; i < stateStack.size(); i++)
    {
        stateStack.top().get()->Destroy();
        stateStack.pop();
    }
}

void CStateManager::PushState(const CGameState& state)
{
    numStates++;
    stateStack.push(state.clone());
    stateStack.top().get()->Init();
}

void CStateManager::PopState(void)
{
    numStates--;
    stateStack.top().get()->Destroy();
    stateStack.pop();
}

const std::unique_ptr<CGameState>& CStateManager::Peek(void) const
{
    return stateStack.top();
}
