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
	float health;
	float maxHealth;
	bool dead;
	Camera* camera;
	Mesh mesh;

	Player(PhysicsManager* physicsManager = nullptr, GLFWwindow* window = nullptr, glm::vec3 position = glm::vec3(0.0f));
	// 
	void PollMouseMovement(float xoffset, float yoffset, bool mouseHidden, GLboolean constrainPitch = true);

	void PollMouseButtons(int button, int action);
	// any constant updates can be put here
	void Update();
	// any frame rate dependent updates can be put here
	void FixedUpdate(float deltatime);

	float movementSpeed;
	float mouseSensitivity;
	bool noclip;

private:
	// Executes any movement calls
	void PollMovement(float deltatime);
	//
	void PollCollision(float deltatime);
	//
	void ResolveCollision(DistTriangle triangle, float deltatime);

	float collisionThreshold;
	bool grounded;
	bool jumped;
	glm::vec3 front;
	glm::vec3 cameraOffset;
	GLFWwindow* window;
	PhysicsManager* physicsManager;
};