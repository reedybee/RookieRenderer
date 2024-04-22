#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader/Shader.h"
#include "camera/Camera.h"
#include "player/Player.h"
#include "mesh/mesh.h"
#include "texture/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "util/util.h"

float lastX = windowResolution.x / 2.0f;
float lastY = windowResolution.y / 2.0f;
bool firstMouse = true;

Player player;
bool mouseHidden = false;

Mesh cube;

static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
		mouseHidden = !mouseHidden;
	}
	if (key == GLFW_KEY_F11  && action == GLFW_RELEASE) {
		ToggleFullscreen(window, WINDOWED_FULLSCREEN, fullscreenResolution);
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		glfwSetWindowShouldClose(window, true);
	}
}

static void mousebuttonCallback(GLFWwindow* window, int button, int action, int mods) {
	player.PollMouseButtons(button, action);
}

static void frambuffersizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	windowResolution.x = width;
	windowResolution.y = height;
}

static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	player.PollMouseMovement(xoffset, yoffset, mouseHidden);
}

// main execution of the program. returns 0 on success, 1 on fail;
int main(int argc, char* argv[]) {
	printf("Application Started\n");
	if (!InitGLFW(4.6, GLFW_OPENGL_CORE_PROFILE))
		return 1;

	GetMonitorResolutions();

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	windowResolution.w = mode->width;
	windowResolution.h = mode->height;
	fullscreenResolution.w = windowResolution.w;
	fullscreenResolution.h = windowResolution.h;
	GLFWwindow* window = glfwCreateWindow(int(windowResolution.w/ 1.5f), int(windowResolution.h / 1.5f), "RookieRenderer", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetFramebufferSizeCallback(window, frambuffersizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetMouseButtonCallback(window, mousebuttonCallback);

	if (!InitGlad())
		return 1;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	
	player = Player(window, glm::vec3(0.0f, 10.0f, 0.0f));
	player.camera->type = CAMERA_TYPE_FIRST_PERSON;

	player.camera->lightPosition = glm::vec3(0.0f, 5.0f, 0.0f);
	Mesh devMesh = Mesh("resources/objects/devscene", &player);
	devMesh.position = glm::vec3(0.0f, 0.0f, 0.0f);
	devMesh.shader = Shader("resources/shaders/unlit/unlitvertex.glsl", "resources/shaders/unlit/unlitfragment.glsl");
	devMesh.colour = glm::vec3(1.0f, 0.2f, 0.2f);
	devMesh.tag = MESH_ENVIRONMENT | MESH_COLLIDER;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// hides mouse for cleaner look in game
		if (mouseHidden)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (!mouseHidden)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// draws objects to scene
		devMesh.Draw(GetAspectRatio());

		player.Update();
		FixedUpdate([]{
			player.FixedUpdate(tickRate);
		});
		// renders contents and polls events
		glfwPollEvents();
		glfwSwapBuffers(window);
		// 0 -> uncapped, 1 -> screen refresh, 2+ -> screenrefresh / num;
		glfwSwapInterval(0);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	printf("\nApplication Terminated");
	return 0;
}