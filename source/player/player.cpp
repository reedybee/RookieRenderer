#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera/Camera.h"
#include "player/Player.h"
#include "physics/Physics.h"
#include "util/util.h"

Player::Player(PhysicsManager* physicsManager, GLFWwindow* window, glm::vec3 position) {
	this->position = position;
	rotation = glm::vec3(0.0f);
	camera = new Camera(glm::vec3(position.x, position.y, position.z));
	movementSpeed = 10.0f;
	mouseSensitivity = 0.1f;
	this->window = window;
	front = glm::vec3(0.0f, 0.0f, 1.0f);
	cameraOffset = glm::vec3(0.0f, 2.0f, 0.0f);
	jumped = false;
	collisionThreshold = 0.5f;
	noclip = false;
	velocity = glm::vec3(0.0f);
	grounded = false;
	health = 100;
	maxHealth = 100;
	dead = false;
	this->physicsManager = physicsManager;
}

void Player::PollMovement(float deltatime) {
	float velocity = deltatime * movementSpeed;
	camera->position = this->position + cameraOffset;
	this->front = glm::normalize(glm::vec3(camera->front.x, 0, camera->front.z));

	if (camera->type == CAMERA_TYPE_DEBUG) {
		if (glfwGetKey(window, GLFW_KEY_W)) {
			position += camera->front * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			position -= camera->front * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			position -= camera->right * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			position += camera->right * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			position += camera->worldUp * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
			position -= camera->worldUp * velocity;
		}
	}
	if (camera->type == CAMERA_TYPE_FIRST_PERSON) {
		if (glfwGetKey(window, GLFW_KEY_W)) {
			position += this->front * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			position -= this->front * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			position -= camera->right * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			position += camera->right * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && grounded) {
			this->velocity.y += 5.0f;
			grounded = false;
		}
	}
}

void Player::PollMouseMovement(float xoffset, float yoffset, bool mouseHidden, GLboolean constrainPitch) {
	if (mouseHidden) {
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		camera->pitch += yoffset;	// up/down
		camera->yaw += xoffset;		// left/right

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (camera->pitch > 89.0f)
				camera->pitch = 89.0f;
			if (camera->pitch < -89.0f)
				camera->pitch = -89.0f;
		}
		camera->UpdateCameraVectors();
	}
}

void Player::PollMouseButtons(int button, int action) {
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
		unsigned int tag;
		Player hitplayer;
		glm::vec3 point = physicsManager->FindPointDirection(this->camera->position, this->camera->front, &tag, &hitplayer);
		if (tag & MESH_ENVIRONMENT) {
			printf("Enivronment Hit at ");
			DisplayVec3(point);
		}
		if (tag & MESH_ENEMY) {
			printf("Enemy Hit at ");
			DisplayVec3(point);
		}
	}
}

void Player::PollCollision(float deltatime) {
	if (!noclip) {
		std::vector<DistTriangle> triangles = physicsManager->PollDistances(this->position);
		for (const DistTriangle& triangle : triangles) {
			if (triangle.distance < collisionThreshold && triangle.tag & MESH_COLLIDER) {
				ResolveCollision(triangle, deltatime);
				float groundDot = glm::dot(triangle.normal, this->camera->worldUp);
				//velocity = glm::reflect(velocity, triangle.normal); // leaving here cause funny
				if (groundDot > 0.70f) {
					grounded = true;
					velocity.y = 0.0f;
				}
				else {
					grounded = false;
				}
			}
		}
		this->position += velocity * deltatime;
		this->velocity.y -= physicsManager->gravity * deltatime;
	}
}

void Player::ResolveCollision(DistTriangle triangle, float deltatime) {
	// the dot between the triangle and the world up
	float surfaceDot = glm::dot(triangle.normal, this->camera->worldUp);
	if (surfaceDot == 0) {
	}
	// should be only slanted walls/floors
	if (surfaceDot != 0 && surfaceDot != 1 && surfaceDot != -1) {
		triangle.normal = glm::vec3(0, triangle.normal.y, 0);
		glm::normalize(triangle.normal);
	}
	//std::cout << triangle.normal.x << " " << triangle.normal.y << " " << triangle.normal.z << "\n";
	float depth = collisionThreshold - triangle.distance;
	glm::vec3 targetPosition = triangle.normal * glm::vec3(depth);
	// todo: recheck this, sometimes the target position is a NAN, 
	// this just ignores it for now maybe?
	if (isnan(targetPosition.x) || isnan(targetPosition.y) || isnan(targetPosition.z)) {
		std::cout << "Ignored NAN.\n";
		return;
	}
	this->position += targetPosition;
}

void Player::Update() {
	if (health <= 0) {
		dead = true;
		std::cout << "Player Died.\n";
	}
}

void Player::FixedUpdate(float deltatime) {
	// check for collisions
	this->PollCollision(tickRate);
	// player movement
	this->PollMovement(tickRate);
}