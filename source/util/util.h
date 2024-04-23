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

// CAUTION!!! avoid using customs headers. may break code

#define M_TAU 2 * M_PI

static int initialWindowWidth;
static int initialWindowHeight;

static int currentWindowWidth;
static int currentWindowHeight;

static int lastWindowWidth;
static int lastWindowHeight;

static bool fullscreen = false;

// rate at which logic will be updated, 60 times/second.
static float tickRate = 1.0f / 60.0f;
static float accumulatedTime = 0.0f;
static float currentTime = (float)glfwGetTime();
static float newTime, frameTime;

struct DistTriangle {
	float distance;
	glm::vec3 normal;
	unsigned int tag;
};

struct RayPoint {
	glm::vec3 position;
	unsigned int tag;
};

#define WINDOWED_FULLSCREEN 0
#define FULLSCREEN 1

static void ToggleFullscreen(GLFWwindow* window, int fullscreenType, int xResolution, int yResolution) {
	if (!fullscreen) {
		if (fullscreenType == WINDOWED_FULLSCREEN) {
			glfwMaximizeWindow(window);
			glfwSetWindowAttrib(window, GLFW_DECORATED, false);
			glfwSetWindowSize(window, xResolution, yResolution);
		}
		if (fullscreenType == FULLSCREEN) {
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), NULL, NULL, initialWindowWidth, initialWindowHeight, NULL);
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
	return (float)currentWindowWidth / (float)currentWindowHeight;
}