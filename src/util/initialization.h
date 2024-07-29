#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define SSAA_SAMPLE_SIZE 16
// initializes GLFW
static int InitGLFW(double version, int openglProfile) {
	double major = 0.0;
	double minor = modf(version, &major) * 10;
	const char* error;
	int initCode = glfwInit();
	// check for errors initiallizing glfw
	if (initCode == GLFW_FALSE) {
		while (glfwGetError(&error) != GLFW_NO_ERROR) {
			std::cout << "GLFW failed to Initialize\n Error Code: " << error << "\n";
		}
		return 0;
	}
	// sets the windows hints 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, (int)major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, (int)minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, openglProfile);
	glfwWindowHint(GLFW_SAMPLES, SSAA_SAMPLE_SIZE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	std::cout << "GLFW Initialized: Version " << major << "." << minor << "\n";
	return 1;
}
// initializes OpenGL
static int InitGlad() {
	int code = gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
	if (!code)
		std::cout << "Failed to initialize GLAD \n";
	if (code)
		std::cout << "GLAD initialized \n\n";
	return code;
}