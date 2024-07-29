#pragma once
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gfx/camera.h"

enum CAMERA_TYPE {
	CAMERA_TYPE_FIRST_PERSON,
	CAMERA_TYPE_DEBUG,
};

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float fov;
	float yaw, pitch;
	float nearPlane, farPlane;

	Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(float aspect) const;

	// updates front, right and up vectors using the updated angles
	void UpdateCameraVectors();

	glm::vec3 lightPosition;

	int type;
private:
protected:
};