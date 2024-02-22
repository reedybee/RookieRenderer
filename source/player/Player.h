#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera/Camera.h"
#include "physics/Physics.h"

class Player {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	Camera* camera;

	Player(GLFWwindow* window = nullptr, glm::vec3 position = glm::vec3(0.0f));
	// Executes any movement calls
	void PollMovement(float deltatime);
	
	void PollMouse(float xoffset, float yoffset, bool mouseHidden, GLboolean constrainPitch = true);

	void PollCollision(PhysicsManager* physicsManager);

	float movementSpeed;
	float mouseSensitivity;

private:
	void ResolveCollision(DistTriangle triangle);

	bool jumped;
	glm::vec3 front;
	GLFWwindow* window;
	float collisionThreshold;
};