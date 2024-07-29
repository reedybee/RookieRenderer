#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/util.h"
#include "gfx/camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
	this->position = position;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right = glm::vec3(0);
	this->up = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->fov = 90.0f;
	this->type = CAMERA_TYPE_DEBUG;
	nearPlane = 0.01f;
	farPlane = 1000.0f;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspect) const {
	return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}
	
void Camera::UpdateCameraVectors() {
	front = glm::normalize(glm::vec3(
	cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
	sin(glm::radians(pitch)),
	sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
	// also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}
