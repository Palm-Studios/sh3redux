/** @file
 *
 *
 *  @copyright 2016-2019  Palm Studios
 *
 *  @date 26-3-2019
 *
 *  @author Jesse Buhagiar [quaker762]
 */

#ifndef _INTRO_HPP_
#define _INTRO_HPP_

#include "SH3/engine/gamestate.hpp"
#include "SH3/graphics/texture.hpp"
#include "SH3/system/shader.hpp"
#include "SH3/system/glvertexarray.hpp"
#include "SH3/system/glvertexbuffer.hpp"

namespace sh3 { namespace state {

/**
 * Game intro state. Is in charge of displaying the Konami KCET logo, game warning
 * ("This game contains scenes....") and the intro video. Basically just draws
 * a bunch of fun stuff!
 */
class CIntroState : public CGameState
{
public:
    CIntroState(CStateManager& mgr) : CGameState(mgr)
    {
        name = "sh3_state_intro";
        id = 1;
    }

    CIntroState(const CIntroState& rhs) : CGameState(rhs)
    {

    }

    /**
     *
     */
    ~CIntroState()
    {
    }

    virtual std::unique_ptr<CGameState> clone() const override
    {
        return std::make_unique<CIntroState>(*this);
    }

    virtual void Init(void) noexcept;
    virtual void Destroy(void) noexcept;
    virtual void Update(void) noexcept;
    virtual void Render(void) noexcept;
    virtual void InputHandler(const SDL_Event& event) noexcept;

private:
    sh3::graphics::CTexture konami1;    /**< First konami logo */
    sh3::graphics::CTexture kcet;       /**< KCET Logo */
    sh3::graphics::CTexture warning;    /**< Warning logo */

    std::size_t             ticks;      /**< Tick execution counter */
    float                   alpha = 0.0f;
    int                     numTimes = 0;
    sh3::gl::CShader        shader;

    // OpenGL related structures
    using Quad = sh3::gl::CVertexArray;

    Quad quadVao2;
    sh3::gl::CVertexBuffer      vertBuff;
    sh3::gl::VertexAttribute    vertAttribs;
    sh3::gl::CVertexBuffer      uvBuff;
    sh3::gl::VertexAttribute    uvAttribs;
};


}}








#endif /* INCLUDE_SH3_ENGINE_STATE_INTRO_HPP_ */
