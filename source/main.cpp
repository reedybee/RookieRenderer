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
#include "player/Enemy.h"
#include "mesh/mesh.h"
#include "texture/Texture.h"
#include "physics/Physics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "util/util.h"
#include "util/server.h"

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;

Player player;
PhysicsManager physicsManager = PhysicsManager();
bool mouseHidden = false;

Mesh cube;

static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
		mouseHidden = !mouseHidden;
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

	player.PollMouseMovement(xoffset, yoffset, mouseHidden);
}

// main execution of the program. returns 0 on success, 1 on fail;
int main(int argc, char* argv[]) {
	printf("Application Started\n");
	if (!InitGLFW(4.6, GLFW_OPENGL_CORE_PROFILE))
		return 1;

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	windowWidth = mode->width;
	windowHeight = mode->height;
	GLFWwindow* window = glfwCreateWindow(int(mode->width / 1.5f), int(mode->height / 1.5f), "RookieRenderer", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetFramebufferSizeCallback(window, frambuffersizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetMouseButtonCallback(window, mousebuttonCallback);

	if (!InitGlad())
		return 1;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	
	player = Player(&physicsManager, window, glm::vec3(0.0f, 10.0f, 0.0f));
	player.camera->type = CAMERA_TYPE_FIRST_PERSON;

	player.camera->lightPosition = glm::vec3(0.0f, 5.0f, 0.0f);
	Mesh devMesh = Mesh("resources/objects/devscene.obj", player.camera);
	devMesh.position = glm::vec3(0.0f, 0.0f, 0.0f);
	devMesh.shader = Shader("resources/shaders/unlit/unlitvertex.glsl", "resources/shaders/unlit/unlitfragment.glsl");
	devMesh.colour = glm::vec3(1.0f, 0.2f, 0.2f);
	devMesh.tag = MESH_ENVIRONMENT | MESH_COLLIDER;

	Mesh scaleMesh = Mesh("resources/objects/scale.obj", player.camera);
	
	scaleMesh.position = glm::vec3(5.0f, 1.0f, 6.0f);
	scaleMesh.shader = Shader("resources/shaders/unlit/unlitvertex.glsl", "resources/shaders/unlit/unlitfragment.glsl");
	scaleMesh.colour = glm::vec3(1.0f, 0.2f, 0.2f);
	scaleMesh.tag = MESH_ENEMY;

	physicsManager.AddMesh(&devMesh);
	physicsManager.AddMesh(&scaleMesh);

	Enemy enemy = Enemy(glm::vec3(0.0f), glm::vec3(0.0f), "resources/objects/elf.obj", "resources/shaders/unlit/unlitvertex.glsl", "resources/shaders/unlit/unlitfragment.glsl", player.camera);
	enemy.scale = glm::vec3(0.025f);
	enemies.push_back(&enemy);

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
		//scaleMesh.Draw(GetAspectRatio());

		DrawEnemies(GetAspectRatio());

		player.Update();
		FixedUpdate([]{
			player.FixedUpdate(tickRate);
		}, true);
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