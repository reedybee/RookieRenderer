#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader/Shader.h"
#include "camera/Camera.h"
#include "player/Player.h"
#include "mesh/mesh.h"
#include "model/Model.h"

int windowWidth;
int windowHeight;

float lasttime = (float)glfwGetTime();
float deltatime = (float)glfwGetTime();
float lastFrame = 0.0f;
float framerate = 144;

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;

Player player;
bool mouseHidden = false;

float GetAspectRatio() {
	return (float)windowWidth / (float)windowHeight;
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
		mouseHidden = !mouseHidden;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		glfwSetWindowShouldClose(window, true);
	}
}

void frambuffersizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);


	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	player.PollMouse(xoffset, yoffset, mouseHidden);
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
	glfwSetCursorPosCallback(coreWindow, mouseCallback);
	glfwSetKeyCallback(coreWindow, keyboardCallback);
	
	windowWidth = mode->width;
	windowHeight = mode->height;

	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) {
		std::cout << "Failed to initialize GLAD \n";
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	player = Player(coreWindow, glm::vec3(0.0f,0.0f, 0.0f));

	std::vector<glm::vec3> planeVertices = {
		{ 0.5f,0.0f, 0.5f },
		{ 0.5f,0.0f,-0.5f },
		{-0.5f,0.0f, 0.5f },
		{-0.5f,0.0f,-0.5f },
	};
	std::vector<unsigned int> planeIndices = {
		0, 1, 2,
		1, 2, 3,
	};

	Model plane = Model(player.camera, "resource/shaders/unlit/unlitvertex.glsl", "resource/shaders/unlit/unlitfragment.glsl", planeVertices, planeIndices);
	plane.position = glm::vec3(0.0f, -3.0f, 0.0f);
	plane.scale = glm::vec3(100.0f, 1.0f, 100.0f);

	std::vector<glm::vec3> cubeVertices = {
		{ -0.5f, 0.5f, -0.5f }, // back top left
		{  0.5f, 0.5f, -0.5f }, // back top right
		{  0.5f,-0.5f, -0.5f }, // back bottom right
		{ -0.5f,-0.5f, -0.5f }, // back bottom left
		{ -0.5f, 0.5f,  0.5f }, // front top left
		{  0.5f, 0.5f,  0.5f }, // front top right
		{  0.5f,-0.5f,  0.5f }, // front bottom right
		{ -0.5f,-0.5f,  0.5f }, // front bottom left
	};

	std::vector<unsigned int> cubeIndices = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		0, 3, 4,
		3, 4, 7,
		1, 2, 5,
		2, 5, 6,
		2, 3, 6,
		3, 6, 7,
		0, 1, 5,
		0, 4, 5,
	};

	Model cube = Model(player.camera, "resource/shaders/unlit/unlitvertex.glsl", "resource/shaders/unlit/unlitfragment.glsl", cubeVertices, cubeIndices);
	
	while (!glfwWindowShouldClose(coreWindow)) {
		glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (mouseHidden)
			glfwSetInputMode(coreWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (!mouseHidden)
			glfwSetInputMode(coreWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		cube.Draw(GetAspectRatio());
		plane.Draw(GetAspectRatio());

		player.PollMovement(deltatime);

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
		lasttime += 1.0f / framerate;
	}
	glfwTerminate();
}