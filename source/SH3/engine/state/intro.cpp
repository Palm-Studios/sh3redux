/** @file
 *
 *
 *  @copyright 2016-2019  Palm Studios
 *
 *  @date 26-3-2019
 *
 *  @author Jesse Buhagiar [quaker762]
 */
#include "SH3/engine/gamestate.hpp"
#include "SH3/engine/state/intro.hpp"

using namespace sh3::state;

// Quad Co-Ordinates
static GLfloat vert_buffer[] =
{
    -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
};

static GLfloat uv_buffer[] =
{
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};


void CIntroState::Init(void) noexcept
{
    /**
     * TODO: Should this be put in CRenderContext on init???
     */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shader.Load("image");

    // Load textures
    kcet.Load("data/pic/kcet.bmp");
    konami1.Load("data/pic/konami.bmp");
    warning.Load(mft, "data/pic/sy/sys_warning.tex");

    // Upload geometry data to the GPU
    vertBuff.BufferData(sh3::gl::CVertexBuffer::BufferTarget::ARRAY_BUFFER, sizeof(vert_buffer), vert_buffer, sh3::gl::CVertexBuffer::BufferUsage::STATIC_DRAW);
    vertAttribs.idx = 0;
    vertAttribs.normalize = false;
    vertAttribs.offset = 0;
    vertAttribs.size = 3;
    vertAttribs.stride = 0;
    quadVao2.BindAttribute(vertAttribs, vertBuff, sh3::gl::VertexAttribute::AttributeType::FLOAT);

    uvBuff.BufferData(sh3::gl::CVertexBuffer::BufferTarget::ARRAY_BUFFER, sizeof(uv_buffer), uv_buffer, sh3::gl::CVertexBuffer::BufferUsage::STATIC_DRAW);
    uvAttribs.idx = 1;
    uvAttribs.normalize = false;
    uvAttribs.offset = 0;
    uvAttribs.size = 2;
    uvAttribs.stride = 0;
    quadVao2.BindAttribute(uvAttribs, uvBuff, sh3::gl::VertexAttribute::AttributeType::FLOAT);

    numTimes = 0;
    ticks = 0;
}

void CIntroState::Destroy(void) noexcept
{
    glDisable(GL_BLEND); // Disable blending for now
}

void CIntroState::Update(void) noexcept
{
    ticks++;
    if(ticks <= 90)
        alpha += 0.02f;

    if(ticks > 270)
        alpha -= 0.02f;

    if(ticks > 360)
    {
        numTimes++;
        alpha = 0.0f;
        ticks = 0;
    }

    shader.SetUniform<float>("blendAlpha", alpha);
}

void CIntroState::Render(void) noexcept
{
    glClear(GL_COLOR_BUFFER_BIT);
    shader.Bind();

    if(numTimes == 0)
        konami1.Bind(GL_TEXTURE0);
    else if(numTimes == 1)
        kcet.Bind(GL_TEXTURE0);
    else
        warning.Bind(GL_TEXTURE0);

    quadVao2.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CIntroState::InputHandler(const SDL_Event& event) noexcept
{

}
