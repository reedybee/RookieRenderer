#include <iostream>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader/Shader.h"
#include "camera/Camera.h"
#include "player/Player.h"

int windowWidth;
int windowHeight;

float lasttime = glfwGetTime();
float deltatime = glfwGetTime();
float lastFrame = 0.0f;
float framerate = 144;

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;
float xoffset;
float yoffset;


float GetAspectRatio() {
	return (float)windowWidth / (float)windowHeight;
}

void frambuffersizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);


	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	xoffset = 0.0f;
	yoffset = 0.0f;

	xoffset = xpos - lastX;
	yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
}

int main(int argc, char* argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	GLFWwindow* coreWindow = glfwCreateWindow(mode->width / 2, mode->height / 2, "RookieRenderer", NULL, NULL);
	glfwMakeContextCurrent(coreWindow);
	glfwSetFramebufferSizeCallback(coreWindow, frambuffersizeCallback);
	glfwSetCursorPosCallback(coreWindow, mouse_callback);
	windowWidth = mode->width;
	windowHeight = mode->height;

	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
		return -1;

	Player player = Player(coreWindow, glm::vec3(0.0f,0.0f, 4.0f));
	Shader standardUnlit = Shader(&player.camera, "resource/shaders/unlit/unlitvertex.glsl", "resource/shaders/unlit/unlitfragment.glsl");

	glm::vec3 vertices[] = {
		{ 1.0f, 0.0f, 0.0f },
		{-1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f,-1.0f, 0.0f },
	};

	int indices[] = {
		0, 1, 2,
		0, 1, 3
	};

	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_READ);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	while (!glfwWindowShouldClose(coreWindow)) {
		glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		standardUnlit.UpdateMatrices(GetAspectRatio());

		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, NULL);

		player.PollMovement(deltatime);
		player.PollMouse(xoffset, yoffset, true);
		std::cout << xoffset << " " << yoffset << "\n";

		glfwPollEvents();
		glfwSwapBuffers(coreWindow);

		// calculates deltatime
		float currentFrame = (float)glfwGetTime();
		deltatime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// waits for amount of frames as defined in framerate
		while (glfwGetTime() < lasttime + 1.0 / framerate) {
			std::this_thread::yield();
		}
		lasttime += 1.0 / framerate;
	}
	glfwTerminate();
}