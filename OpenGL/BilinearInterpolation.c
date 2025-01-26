#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* �������� */
const unsigned int WIN_W = 500; // �������� ���� ũ��
const unsigned int WIN_H = 500; // �������� ���� ũ��

const char* vertexShaderFileName = "VertexShader.vert";
const char* fragmentShaderFileName = "FragmentShader.frag";

GLuint vertexShaderID = 0;
GLuint fragmentShaderID = 0;
GLuint ShaderProgramID = 0;
GLuint vertexBufferObjectID = 0;
GLuint vertexArrayObjectID = 0;

/*
loadSourceCode :
���� �̸��� �Ű� ������ �޾� ������ �а� ���ڿ��� ��ȯ�ϴ� �Լ�
*/
const char* loadSourceCode(const char* filename) {
	FILE* fp = fopen(filename, "r"); // ������ �б���(r)�� ���� 
	if (fp == NULL) { // ������ ��� ���� �޽����� ���
		fprintf(stderr, "Error: cannot open \"%s\"\n", filename);
		return NULL;
	}
	fseek(fp, 0, SEEK_END); // ������ �� ������ �̵�
	size_t len = ftell(fp); // ������ ���� ��ġ�� ����(����Ʈ������ ��ȯ, ������ ũ��)
	rewind(fp); // �ٽ� ó������ �̵�
	char* buf = (char*)malloc(sizeof(char) * (len + 4)); // ������ ������ ���� �޸� �Ҵ�
	size_t size = fread(buf, sizeof(char), len, fp); // ������ �о �޸𸮿� ����
	fclose(fp); // ���� �ݱ�
	buf[size] = '\0'; // ������ ���� �ι��� �߰�
	return (const char*)buf;  // ���ڿ� ��ȯ
}

/*
initShader :
���̴� ���α׷��� �������ϰ� ��ũ�ϴ� �Լ�
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

/* �Է� ���� */
GLfloat vertices[] = {
	// ��ġ(vec4)/��(vec4)
	0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
};

void drawFunc(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vertexArrayObjectID);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFinish();
}

// Refresh Callback �Լ��� ����
void refreshFunc(GLFWwindow* window) {
	drawFunc();
	glfwSwapBuffers(window);
}

// Keyboard Callback �Լ��� ����
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
	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "Bilinear Interpolation", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetWindowRefreshCallback(window, refreshFunc);
	glfwSetKeyCallback(window, keyFunc);

	initShader();

	glGenVertexArrays(1, &vertexArrayObjectID);
	glGenBuffers(1, &vertexBufferObjectID);
	glBindVertexArray(vertexArrayObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window)) {
		drawFunc();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
