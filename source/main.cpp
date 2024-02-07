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

int windowWidth;
int windowHeight;

float lasttime = glfwGetTime();
float deltatime = glfwGetTime();
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

	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
		return -1;
	glEnable(GL_DEPTH_TEST);
	player = Player(coreWindow, glm::vec3(0.0f,0.0f, 4.0f));
	Shader standardUnlit = Shader(player.camera, "resource/shaders/unlit/unlitvertex.glsl", "resource/shaders/unlit/unlitfragment.glsl");

	Shader planeUnlit = Shader(player.camera, "resource/shaders/unlit/unlitvertex.glsl", "resource/shaders/unlit/unlitfragment.glsl");
	planeUnlit.scale = glm::vec3(3.0f);
	planeUnlit.position = glm::vec3(0.0f, -1.0f, 0.0f);

	std::vector<glm::vec3> planeVertices = {
		{ 1.0f,0.0f, 1.0f },
		{ 1.0f,0.0f,-1.0f },
		{-1.0f,0.0f, 1.0f },
		{-1.0f,0.0f,-1.0f },
	};
	std::vector<unsigned int> planeIndices = {
		0, 1, 2,
		1, 2, 3,
	};

	Mesh mesh = Mesh(planeVertices, planeIndices);

	std::vector<glm::vec3> cubeVertices = {
		{ 1.0f, 0.0f, 0.0f },
		{-1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f,-1.0f, 0.0f },
		{ 0.0f, 0.0f,-1.0f },
		{ 0.0f, 0.0f, 1.0f },
	};

	std::vector<unsigned int> cubeIndices = {
		0, 1, 2,
		0, 1, 3,
		0, 1, 4,
		0, 1, 5,
		3, 2, 5,
		3, 2, 4
	};

	Mesh cube = Mesh(cubeVertices, cubeIndices);
	
	while (!glfwWindowShouldClose(coreWindow)) {
		glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (mouseHidden)
			glfwSetInputMode(coreWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (!mouseHidden)
			glfwSetInputMode(coreWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		standardUnlit.UpdateMatrices(GetAspectRatio());
		cube.Draw();

		planeUnlit.UpdateMatrices(GetAspectRatio());
		mesh.Draw();

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
		lasttime += 1.0 / framerate;
	}
	glfwTerminate();
}