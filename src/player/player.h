#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gfx/camera.h"
#include "gfx/mesh.h"
#include "game/state.h"

enum collisionType {
	VERTICAL,
	SLANTED,
};

class Player {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 velocity;
	glm::vec3 gravity;
	float jumpForce;
	float health;
	float maxHealth;
	bool dead;
	Camera* camera;
	Mesh mesh;

	Player(glm::vec3 position = glm::vec3(0.0f));
	// 
	void PollMouseMovement(bool mouseHidden, GLboolean constrainPitch = true);

	void PollMouseButtons(int button, int action);
	// any constant updates can be put here
	void Update();
	// any frame rate dependent updates can be put here
	void FixedUpdate(float deltatime);

	bool IsGrounded() const;

	bool IsMoving() const;

	glm::vec3 GetFront() const;
	void SetFront(glm::vec3 front);

	float movementSpeed;
	float mouseSensitivity;
	bool noclip;

private:
	// Executes any movement calls
	void PollMovement(float deltatime);
	//
	void PollCollision(float deltatime);
	//
	void ResolveCollision(glm::vec4 normal, float deltatime);

	float collisionThreshold;
	bool grounded;
	bool moving;
	glm::vec3 front;
	glm::vec3 cameraOffset;
	glm::vec3 lastPosition;

	float lastX, lastY;
	bool firstMouse;
};