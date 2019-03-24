#version 420

in vec2 uv;
out vec4 color;

uniform sampler2D texSampler;
uniform float blendAlpha = 1.0f;
void main()
{
    color = texture(texSampler, uv);
    color.a = blendAlpha;
}
