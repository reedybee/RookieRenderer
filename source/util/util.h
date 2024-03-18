#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>

// CAUTION!!! avoid using customs headers. may break code

#define SSAA_SAMPLE_SIZE 16
#define M_TAU 2 * M_PI

static int windowWidth, windowHeight;

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

// prints a vector 3 to the standard outputS
static void DisplayVec3(glm::vec3 vector) {
	std::cout << "x: " << vector.x << " y: " << vector.y << " z: " << vector.z << "\n";
}

// converts radians to degrees
template <typename T> T degrees(T num) {
	return (num * 180) / M_PI;
}

// converts degrees to radians
template <typename T> T radians(T num) {
	return (num * M_PI) / 180;
}

// finds the lowest value in an array, and optionally returns the index
template <typename T> T FindLowestValue(std::vector<T> values) {
	T best = std::numeric_limits<T>::max();
	T current = 0;
	T last = 0;
	for (int i = 0; i < values.size(); i++) {
		current = values[i];
		if (current < best) {
			best = current;
			last = current;
		}
	}
	return best;
}

// finds the max value in an array, and optionally returns the index
template <typename T> T FindHighestValue(std::vector<T> values) {
	T best = std::numeric_limits<T>::min();
	T current = 0;
	T last = 0;
	for (int i = 0; i < values.size(); i++) {
		current = values[i];
		if (current > best) {
			best = current;
			last = current;
		}
	}
	return best;
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
	return (float)windowWidth / (float)windowHeight;
}

// reads contents of a file into a string.
static std::string ReadFromFile(const char* filepath) {
	std::string contents;
	std::ifstream file(filepath);

	std::string line;

	if (!file.good()) {
		std::cout << "Failed to open file " << filepath << "\n";
	}
	if (file.is_open()) {
		while (std::getline(file, line)) {
			line.append("\n");
			contents.append(line);
		}
	}
	file.close();

	return contents;
}

// calcutates the dot of the two same numbers
static float dot2(glm::vec3 a) {
	return glm::dot(a, a);
}

// returns the signed distance to the triangle specified
// thanks to https://iquilezles.org/articles/distfunctions/
static float sdfTriangle(glm::vec3 position, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	glm::vec3 ba = p2 - p1; glm::vec3 pa = position - p1;
	glm::vec3 cb = p3 - p2;	glm::vec3 pb = position - p2;
	glm::vec3 ac = p1 - p3;	glm::vec3 pc = position - p3;
	glm::vec3 norm = glm::cross(ba, ac);

	return glm::sqrt(
		glm::sign(glm::dot(glm::cross(ba, norm), pa)) +
		glm::sign(glm::dot(glm::cross(cb, norm), pb)) +
		glm::sign(glm::dot(glm::cross(ac, norm), pc)) < 2.0
		?
		glm::min(glm::min(
			dot2(ba * glm::clamp(glm::dot(ba, pa) / dot2(ba), 0.0f, 1.0f) - pa),
			dot2(ba * glm::clamp(glm::dot(ba, pa) / dot2(ba), 0.0f, 1.0f) - pa)),
			dot2(ba * glm::clamp(glm::dot(ba, pa) / dot2(ba), 0.0f, 1.0f) - pa))
		:
		glm::dot(norm, pa) * glm::dot(norm, pa) / dot2(norm));
}