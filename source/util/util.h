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

// CAUTION!!! avoid using customs headers. may break code

#define M_TAU 2 * M_PI

static bool fullscreen = false;

// rate at which logic will be updated, 60 times/second.
static float tickRate = 1.0f / 60.0f;
static float accumulatedTime = 0.0f;
static float currentTime = (float)glfwGetTime();
static float newTime, frameTime;

struct Resolution {
	int x, y;
	int w, h;
	int refreshRate;
};

static Resolution windowResolution;
static Resolution fullscreenResolution;

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

static std::vector<std::string> MonitorResolutionsToString(std::vector<Resolution> resolutions, std::vector<int>* refreshRates) {
	std::vector<std::string> strings;
	for (const Resolution& res : resolutions) {
		std::string resolution = res.w + " x " + res.h;
		strings.push_back(resolution);
		if (refreshRates.size() == 0) {
			refreshRates.push_back(res.refreshRate);
			break;
		}
		for (int i = 0; i < refreshRates.size(); i++) {
			if (refreshRates[i] == res.refreshRate)
				break;
			refreshRates.push_back(res.refreshRate);
		}
	}
	return strings;
}

static std::vector<Resolution> GetMonitorResolutions() {
	int count;
	std::vector<Resolution> resolutions;
	const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
	for (int i = 0; i < count; i++) {
		Resolution res;
		res.w = modes[i].width;
		res.h = modes[i].height;
		res.refreshRate = modes[i].refreshRate;
		resolutions.push_back(res);
	}
	return resolutions;
}

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
