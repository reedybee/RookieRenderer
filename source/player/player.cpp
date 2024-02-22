#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera/Camera.h"
#include "player/Player.h"
#include "physics/Physics.h"
#include "util/util.h"

Player::Player(GLFWwindow* window, glm::vec3 position) {
	this->position = position;
	rotation = glm::vec3(0.0f);
	camera = new Camera(glm::vec3(position.x, position.y, position.z));
	this->movementSpeed = 4.0f;
	this->mouseSensitivity = 0.1f;
	this->window = window;
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->jumped = false;
	this->collisionThreshold = 0.3f;
	this->noclip = false;
	this->velocity = glm::vec3(0.0f);
}

void Player::PollMovement(float deltatime) {
	float velocity = deltatime * movementSpeed;
	camera->position = this->position;
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
			grounded = false;
			this->velocity += glm::vec3(0.0f, 10.0f, 0.0f);
			std::cout << "player jump\n";
		}
	}
}

void Player::PollMouse(float xoffset, float yoffset, bool mouseHidden, GLboolean constrainPitch) {
	if (mouseHidden) {
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		camera->yaw += xoffset;
		camera->pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (camera->pitch > 89.0f)
				camera->pitch = 89.0f;
			if (camera->pitch < -89.0f)
				camera->pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		camera->UpdateCameraVectors();
	}
}

void Player::PollCollision(PhysicsManager* physicsManager, float deltatime) {
	if (!noclip) {
		this->velocity.y -= physicsManager->gravity * deltatime;
		this->position += velocity * deltatime;
		std::vector<DistTriangle> triangles = physicsManager->PollDistances(this->position);
		for (DistTriangle triangle : triangles) {
			if (triangle.distance < collisionThreshold) {
				ResolveCollision(triangle);
				float groundDot = glm::dot(triangle.normal, this->camera->worldUp);
				//velocity = glm::reflect(velocity, triangle.normal); // leaving here cause funny
				if (groundDot > 0.80) {
					velocity = glm::vec3(0.0f);
					std::cout << "Grounded\n";
					grounded = true;
				}
			}
		}
	}
}

void Player::ResolveCollision(DistTriangle triangle) {
	//std::cout << triangle.normal.x << " " << triangle.normal.y << " " << triangle.normal.z << "\n";
	float depth = collisionThreshold - triangle.distance;
	this->position += triangle.normal * glm::vec3(depth);
	//std::cout << depth << "\n";
}
