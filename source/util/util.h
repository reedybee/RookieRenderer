#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static int windowWidth, windowHeight;

static float tickRate = 1.0f / 60.0f;
static float accumulatedTime = 0.0f;
static float currentTime = (float)glfwGetTime();
static float newTime, frameTime;

static void updateTickTime() {
	newTime = (float)glfwGetTime();
	frameTime = newTime - currentTime;
	currentTime = newTime;

	// Accumulate frame time
	accumulatedTime += frameTime;
}

static float GetAspectRatio() {
	return (float)windowWidth / (float)windowHeight;
}

struct DistTriangle {
	float distance;
	glm::vec3 normal;
};

static float dot2(glm::vec3 a) {
	return glm::dot(a, a);
}

static void DisplayVec3(glm::vec3 vector) {
	std::cout << "x: " << vector.x << " y: " << vector.y << " z: " << vector.z << "\n";
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