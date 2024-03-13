#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include "camera/Camera.h"
#include "physics/Physics.h"

static int windowWidth, windowHeight;

static float tickRate = 1.0f / 60.0f;
static float accumulatedTime = 0.0f;
static float currentTime = (float)glfwGetTime();
static float newTime, frameTime;

static struct DistTriangle {
	float distance;
	glm::vec3 normal;
};

static void DisplayVec3(glm::vec3 vector) {
	std::cout << "x: " << vector.x << " y: " << vector.y << " z: " << vector.z << "\n";
}

static glm::vec3 CalculateTriangleCentroid(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	return glm::vec3((v1.x + v2.x + v3.x), (v1.y + v2.y + v3.y), (v1.z + v2.z + v3.z));
}
// calculates a line between an origin and a new point in a direction
static glm::vec3 CalculateLine(glm::vec3 origin, glm::vec3 direction, float length) {
	float newX = length * std::cosf(direction.x);
	float newY = length * std::sinf(direction.x);
	glm::vec3 newVec = glm::vec3(newX + origin.x, newY + origin.y, 0);
	DisplayVec3(newVec);
	return newVec;
}

// calculates a ray between an origin and a collider
static glm::vec3 CalculateColliderRay(Camera* camera, PhysicsManager* physicsManager) {
	float stepSize = 0.5f;
	float stepThreshold = 0.05f;
	float threshold = 0.1f;
	
	glm::vec3 rayPos(0.0f);
	glm::vec3 position = camera->position;
	glm::vec3 rotation = camera->front;

	
	/*
	int count = 0;
	bool posFound = false;
	while (!posFound) {
		std::vector<DistTriangle> triangles = physicsManager->PollDistances(rayPos);
		for (DistTriangle triangle : triangles) {
			if (triangle.distance < threshold) {
				posFound = true;
			} else {
				rayPos = CalculateLine(rayPos, rotation, stepSize);
			}

			if (triangle.distance <= 1.0) {
				stepSize -= stepThreshold * count;
				count++;
			}
		}
	}
	*/
	return rayPos;
}

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

static std::vector<std::string> ReadFromFileIntoArray(const char* filepath) {
	std::vector<std::string> contents;
	std::ifstream file(filepath);

	std::string line;

	if (!file.good()) {
		std::cout << "Failed to open file " << filepath << "\n";
	}
	if (file.is_open()) {
		while (std::getline(file, line)) {
			line.append("\n");
			contents.push_back(line);
		}
	}
	file.close();

	return contents;
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