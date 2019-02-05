#version 450 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertColor;

out vec3 fragColor;
void main(){
	gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
    fragColor = vertColor;
}
