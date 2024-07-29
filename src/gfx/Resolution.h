#pragma once
#include <glfw/glfw3.h>
#include <vector>
#include <string>

struct Resolution {
	unsigned int x, y;
	unsigned int w, h;
	unsigned int refreshRate;
};
// returns a vector of all monitor supported resolutions
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