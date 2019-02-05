#version 420

in vec2 uv;
out vec3 color;

uniform sampler2D texSampler;
void main()
{
  color = texture(texSampler, uv).rgb;
}
