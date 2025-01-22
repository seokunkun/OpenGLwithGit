#version 330 core

layout (location = 0) in vec4 vertexPos;

void main(void) { 
	gl_Position = vertexPos;
}