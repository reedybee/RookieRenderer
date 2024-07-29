#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#define _USE_MATH_DEFINES
#include <math.h>

#include "filesystem.h"
#include "initialization.h"
#include "math.h"
#include "debug.h"

#include "stb_image/stb_image.h"
#include "gfx/Resolution.h"

// CAUTION!!! avoid using customs headers. may break code

#define M_TAU 2 * M_PI

static bool fullscreen = false;

// rate at which logic will be updated, 60 times/second.
static float tickRate = 1.0f / 60.0f;
static float accumulatedTime = 0.0f;
static float currentTime = (float)glfwGetTime();
static float newTime, frameTime;

static Resolution windowResolution;
static Resolution fullscreenResolution;

struct RayPoint {
	glm::vec3 position;
};

#define WINDOWED_FULLSCREEN 0
#define FULLSCREEN 1

static void ToggleFullscreen(GLFWwindow* window, int fullscreenType, Resolution fullscreenResolution) {
	if (!fullscreen) {
		if (fullscreenType == WINDOWED_FULLSCREEN) {
			glfwMaximizeWindow(window);
			glfwSetWindowAttrib(window, GLFW_DECORATED, false);
			glfwSetWindowSize(window, fullscreenResolution.w, fullscreenResolution.h);
			glfwSetWindowPos(window, 0, 0);
		}
		if (fullscreenType == FULLSCREEN) {
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), NULL, NULL, fullscreenResolution.w, fullscreenResolution.h, NULL);
		}
		fullscreen = true;
	} else if (fullscreen) {
		if (fullscreenType == WINDOWED_FULLSCREEN) {
			glfwRestoreWindow(window);
			glfwSetWindowAttrib(window, GLFW_DECORATED, true);
		}
		if (fullscreenType == FULLSCREEN) {
			glfwSetWindowMonitor(window, NULL, NULL, NULL, NULL, NULL, NULL);
		}
		fullscreen = false;
	}
}

// prints a vector 3 to the standard outputS
static void DisplayVec3(glm::vec3 vector) {
	std::cout << "x: " << vector.x << " y: " << vector.y << " z: " << vector.z << "\n";
}

// updates the logic tick timings, avoid making func take longer than 1 / 60s.
static void FixedUpdate(void (*func)(), bool displayFrameRate = false) {
	newTime = (float)glfwGetTime();
	frameTime = newTime - currentTime;
	currentTime = newTime;
	if (displayFrameRate)
		std::cout << (int)(1 / frameTime) << "\n";
	// Accumulate frame time
	accumulatedTime += frameTime;
	while (accumulatedTime >= tickRate) {
		func();
		accumulatedTime -= tickRate;
	}
}

// returns the aspect ratio of the main window.
static float GetAspectRatio() {
	return (float)windowResolution.w / (float)windowResolution.h;
}

static void SetWindowIcon(GLFWwindow* window, const char* filepath) {
	// set window icon
	GLFWimage icon[1];
	icon[0].pixels = stbi_load(filepath, &icon[0].width, &icon[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, icon);
	stbi_image_free(icon[0].pixels);
}
