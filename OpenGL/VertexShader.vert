#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vColor;

void main(void) { 
	gl_Position = aPos;
	vColor = aColor;
}