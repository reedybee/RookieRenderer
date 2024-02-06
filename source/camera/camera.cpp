#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera/Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
	this->position = position;
	this->front = glm::vec3(1.0f, 0.0f, 0.0f);
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right = glm::vec3(0);
	this->up = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->movementSpeed = 3.0f;
	this->mouseSensitivity = 0.1f;
	UpdateCameraVector();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return glm::mat4(0);//glm::perspective(45.0f, );
}

void Camera::UpdateCameraVector() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	// also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(normalize(front), worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}