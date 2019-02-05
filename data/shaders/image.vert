#version 420 
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;

out vec2 uv;
uniform float tFloat;
void main()
{
  gl_Position.xyz = vertexPosition;
  gl_Position.x += tFloat;
  gl_Position.w = 1.0;
  uv = vertexUV;
}
