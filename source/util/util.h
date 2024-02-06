#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

static GLFWwindow* coreWindow;

static int windowWidth;
static int windowHeight;

static float GetAspectRatio() {
	return (float)windowWidth / (float)windowHeight;
}
