#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
#include <stdio.h>
#include <string.h>
#include <malloc.h>

const unsigned int WIN_W = 500; // 윈도우의 가로 크기
const unsigned int WIN_H = 500; // 윈도우의 세로 크기

// 시작 색을 불투명한 검은 색으로 지정
GLfloat clr[4] = { 0.0F, 0.0F, 0.0F, 1.0F };

const char* vertexShaderSource =
"#version 330 core \n\
layout (location = 0) in vec4 vertexPos; \n\
void main(void) { \n\
	gl_Position = vertexPos; \n\
}";

const char* fragmentShaderSource =
"#version 330 core \n\
out vec4 FragColor; \n\
void main(void) { \n\
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n\
}";

GLuint vertexShaderID = 0;
GLuint fragmentShaderID = 0;
GLuint ShaderProgramID = 0;
GLuint vertexBufferObjectID = 0;

void initShader(void) {
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShaderID);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderID);
	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, vertexShaderID);
	glAttachShader(ShaderProgramID, fragmentShaderID);
	glLinkProgram(ShaderProgramID);
	glUseProgram(ShaderProgramID);
}

GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f,
	0.0f, 0.5f, 0.0f, 1.0f
};

void drawFunc(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glGenBuffers(1, &vertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFinish();
}


// Refresh Callback 함수를 정의
void refreshFunc(GLFWwindow* window) {
	drawFunc();
	glfwSwapBuffers(window);
}

// Keyboard Callback 함수를 정의
void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_RELEASE) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	}
}

int main(void) {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "Callback Reference Program", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetWindowRefreshCallback(window, refreshFunc);
	glfwSetKeyCallback(window, keyFunc);
	initShader();
	while (!glfwWindowShouldClose(window)) {
		drawFunc();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
