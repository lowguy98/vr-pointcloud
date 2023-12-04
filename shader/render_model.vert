#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormals;
layout(location = 2) in vec2 vTexcoord;

out vec2 texCoords;

uniform mat4 mvpMatrix;

void main()
{
	// Output position of the vertex, in clip space : MVP * vPosition
	gl_Position =  mvpMatrix * vec4(vPosition, 1.0);

	texCoords = vTexcoord;
}