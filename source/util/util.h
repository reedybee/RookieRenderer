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
