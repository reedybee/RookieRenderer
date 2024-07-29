#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Button {
	bool down;
};

struct Keyboard {
	Button keys[GLFW_KEY_LAST];
};

struct Mouse {
	Button buttons[GLFW_MOUSE_BUTTON_LAST];
	glm::vec2 postion;
	bool hidden;
};

struct Window {
	GLFWwindow* handle;
	glm::ivec2 resolution;

	Keyboard keyboard;
	Mouse mouse;
};

void create_window();
void destroy_window();

extern Window window;