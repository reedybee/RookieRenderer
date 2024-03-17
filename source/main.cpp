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
#include "physics/Physics.h"
#include "sprite/Sprite.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "util/util.h"

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;

Player player;
PhysicsManager physicsManager = PhysicsManager();
bool mouseHidden = false;
bool shouldRender = true;

Mesh cube;

static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
		mouseHidden = !mouseHidden;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		printf("Camera Front: ");  DisplayVec3(player.camera->front);
	}
}

static void frambuffersizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
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

	player.PollMouse(xoffset, yoffset, mouseHidden);
}

int main(int argc, char* argv[]) {
	printf("Application Started\n\n");
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, SSAA_SAMPLE_SIZE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	GLFWwindow* coreWindow = glfwCreateWindow(int(mode->width / 1.5f), int(mode->height / 1.5f), "RookieRenderer", NULL, NULL);
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
	glEnable(GL_MULTISAMPLE);

	player = Player(&physicsManager, coreWindow, glm::vec3(0.0f, 10.0f, 0.0f));
	player.camera->type = CAMERA_TYPE_FIRST_PERSON;

	player.camera->lightPosition = glm::vec3(0.0f, 5.0f, 0.0f);
	Mesh devMesh = Mesh("resources/objects/devscene.obj", player.camera);
	devMesh.position = glm::vec3(0.0f, 0.0f, 0.0f);
	devMesh.shader = Shader("resources/shaders/unlit/unlitvertex.glsl", "resources/shaders/unlit/unlitfragment.glsl");
	devMesh.colour = glm::vec3(0.2f, 0.2f, 0.2f);

	cube = Mesh("resources/objects/boundingcube.obj", player.camera);
	cube.shader = Shader("resources/shaders/unlit/unlitvertex.glsl", "resources/shaders/unlit/unlitfragment.glsl");
	cube.scale = glm::vec3(0.5f);
	cube.colour = glm::vec3(1.0f, 0.0f, 0.0f);

	physicsManager.AddMesh(&devMesh);

	while (!glfwWindowShouldClose(coreWindow)) {
		glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// hides mouse for cleaner look in game
		if (mouseHidden)
			glfwSetInputMode(coreWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (!mouseHidden)
			glfwSetInputMode(coreWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// draws objects to scene
		cube.Draw(GetAspectRatio());
		devMesh.Draw(GetAspectRatio());

		ProcessLogic([]{
			// check for collisions
			player.PollCollision(tickRate);
			// player movement
			player.PollMovement(tickRate);
		});
		// renders contents and polls events
		glfwPollEvents();
		glfwSwapBuffers(coreWindow);
		// 0 -> uncapped, 1 -> screen refresh, 2+ -> screenrefresh / num;
		glfwSwapInterval(1);
	}
	glfwDestroyWindow(coreWindow);
	glfwTerminate();
	printf("\nApplication Terminated");
	return 1;
}