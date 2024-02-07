#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera/Camera.h"
#include "player/Player.h"

Player::Player(GLFWwindow* window, glm::vec3 position) {
	this->position = position;
	rotation = glm::vec3(0.0f);
	camera = new Camera(glm::vec3(position.x, position.y, position.z));
	this->movementSpeed = 4.0f;
	this->mouseSensitivity = 0.1f;
	this->window = window;
}

void Player::PollMovement(float deltatime) {
	float velocity = deltatime * movementSpeed;
	camera->position = this->position;

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
		position += camera->up * velocity;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
		position -= camera->up * velocity;
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
