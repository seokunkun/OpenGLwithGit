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

GLfloat clr[4] = { 0.0F, 0.0F, 0.0F, 1.0F };

const char* vertexShaderFileName = "VertexShader.vert";
const char* fragmentShaderFileName = "FragmentShader.frag";

GLuint vertexShaderID = 0;
GLuint fragmentShaderID = 0;
GLuint ShaderProgramID = 0;
GLuint vertexBufferObjectID = 0;

/*
loadSourceCode :
파일 이름을 매개 변수로 받아 내용을 읽고 문자열로 반환하는 함수
*/
const char* loadSourceCode(const char* filename) { 
	FILE* fp = fopen(filename, "r"); // 파일을 읽기모드(r)로 열기 
	if (fp == NULL) { // 실패할 경우 오류 메시지를 출력
		fprintf(stderr, "Error: cannot open \"%s\"\n", filename);
		return NULL;
	}
	fseek(fp, 0, SEEK_END); // 파일의 맨 끝으로 이동
	size_t len = ftell(fp); // 파일의 현재 위치를 저장(바이트단위로 반환, 파일의 크기)
	rewind(fp); // 다시 처음으로 이동
	char* buf = (char*)malloc(sizeof(char) * (len + 4)); // 파일을 저장할 동적 메모리 할당
	size_t size = fread(buf, sizeof(char), len, fp); // 파일을 읽어서 메모리에 저장
	fclose(fp); // 파일 닫기
	buf[size] = '\0'; // 파일의 끝에 널문자 추가
	return (const char*)buf;  // 문자열 반환
}

/*
initShader :
셰이더 프로그램을 컴파일하고 링크하는 함수
*/
void initShader(void) {
	const char* vertexShaderSource = loadSourceCode(vertexShaderFileName);
	const char* fragmentShaderSource = loadSourceCode(fragmentShaderFileName);

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

	if (vertexShaderSource == NULL || fragmentShaderSource == NULL) {
		fprintf(stderr, "Error: Shader source code could not be loaded.\n");
		free(vertexShaderSource);
		free(fragmentShaderSource);
		return;
	}
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
