#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static int windowWidth, windowHeight;

static float GetAspectRatio() {
	return (float)windowWidth / (float)windowHeight;
}

static float deltatime = (float)glfwGetTime();
static float lasttime = (float)glfwGetTime();
static float lastFrame = 0.0f;
// waits for amount of frames before proceding
static void WaitForFramesElapsed(int maxFramerate) {
	// calculates deltatime
	float currentFrame = (float)glfwGetTime();
	deltatime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	// pauses execution until framerate elapsed
	while (glfwGetTime() < lasttime + 1.0 / maxFramerate) {
		std::this_thread::yield();
	}
	lasttime += 1.0f / maxFramerate;
}

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