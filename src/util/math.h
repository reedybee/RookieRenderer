#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
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