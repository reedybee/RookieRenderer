#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gfx/window.h"
#include "game/state.h"

#include <iostream>

static void window_framebuffer_callback(GLFWwindow* handle, int width, int height) {
	glViewport(0, 0, width, height);
	window.resolution = glm::ivec2(width, height);
}

static void window_keyboard_callback(GLFWwindow* handle, int key, int scancode, int action, int mods) {
	if (key < 0)
		return;

	if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
		window.mouse.hidden = !window.mouse.hidden;
		if (window.mouse.hidden) {
			glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		if (!window.mouse.hidden) {
			glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		glfwSetWindowShouldClose(window.handle, true);
	}

	switch (action) {
		case GLFW_RELEASE:
			window.keyboard.keys[key].down = false;
			break;
		case GLFW_PRESS:
			window.keyboard.keys[key].down = true;
			break;
		default:
			break;
	}
}

static void window_mouse_button_callback(GLFWwindow* handle, int button, int action, int mods) {
	if (button < 0)
		return;

	switch (action) {
		case GLFW_RELEASE:
			window.mouse.buttons[button].down = false;
			break;
		case GLFW_PRESS:
			window.mouse.buttons[button].down = true;
			break;
		default:
			break;
	}
}

static void window_mouse_position_callback(GLFWwindow* handle, double xposIn, double yposIn) {
	window.mouse.postion = glm::vec2(static_cast<float>(xposIn), static_cast<float>(yposIn));
}

void create_window() {
	const char* error; 
	if (!glfwInit()) {
		while (glfwGetError(&error) != GLFW_NO_ERROR) {
			printf("Failed to init GLFW with error %s", error);
		}
		exit(1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window.resolution = glm::ivec2(1920, 1080);

	window.handle = glfwCreateWindow(window.resolution.x, window.resolution.y, "", NULL, NULL);
	if (!window.handle) {
		while (glfwGetError(&error) != GLFW_NO_ERROR) {
			printf("Failed to create window handle with error %s\n", error);
		}
		exit(1);
	}
	glfwMakeContextCurrent(window.handle);

	glfwSetFramebufferSizeCallback(window.handle, window_framebuffer_callback);
	glfwSetKeyCallback(window.handle, window_keyboard_callback);
	glfwSetMouseButtonCallback(window.handle, window_mouse_button_callback);
	glfwSetCursorPosCallback(window.handle, window_mouse_position_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to init GLAD\n");
		exit(1);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void destroy_window() {
	glfwDestroyWindow(window.handle);
	glfwTerminate();
}