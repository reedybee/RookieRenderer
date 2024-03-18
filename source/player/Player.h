#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera/Camera.h"
#include "physics/Physics.h"

enum collisionType {
	VERTICAL,
	SLANTED,
};

class Player {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 velocity;
	Camera* camera;

	Player(PhysicsManager* physicsManager = nullptr, GLFWwindow* window = nullptr, glm::vec3 position = glm::vec3(0.0f));
	// Executes any movement calls
	void PollMovement(float deltatime);
	// 
	void PollMouse(float xoffset, float yoffset, bool mouseHidden, GLboolean constrainPitch = true);
	//
	void PollCollision(float deltatime);

	float movementSpeed;
	float mouseSensitivity;

	bool noclip;

private:
	//
	void ResolveCollision(DistTriangle triangle, float deltatime);

	bool grounded;
	bool jumped;
	glm::vec3 front;
	glm::vec3 cameraOffset;
	GLFWwindow* window;
	float collisionThreshold;
	PhysicsManager* physicsManager;
};