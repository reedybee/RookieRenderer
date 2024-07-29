#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gfx/camera.h"
#include "player/Player.h"
#include "gfx/mesh.h"
#include "util/util.h"

Player::Player(glm::vec3 position) {
	// transforms
	this->position = position;
	this->rotation = glm::vec3(0.0f);
	front = glm::vec3(0.0f, 0.0f, 1.0f);
	cameraOffset = glm::vec3(0.0f, 2.0f, 0.0f);
	// camera
	camera = new Camera(glm::vec3(position.x, position.y, position.z));
	// mouse
	mouseSensitivity = 0.1f;
	// movement
	movementSpeed = 10.0f;
	jumpForce = 7.0f;
	gravity = glm::vec3(0.0f, -9.8f, 0.0f);
	collisionThreshold = 0.5f;
	velocity = glm::vec3(0.0f);
	lastPosition = glm::vec3(0.0f);
	grounded = false;
	moving = false;
	// gameplay
	health = 100;
	maxHealth = 100;
	dead = false;
	noclip = false;

	firstMouse = true;
	lastX = (float)window.resolution.x / 2;
	lastY = (float)window.resolution.y / 2;

	state.SetPlayer(this);
}

void Player::PollMovement(float deltatime) {
	float speed = deltatime * movementSpeed;
	camera->position = this->position + cameraOffset;
	this->front = glm::normalize(glm::vec3(camera->front.x, 0, camera->front.z));
	glm::vec3 maxVelocity = glm::vec3(movementSpeed, 100.0f, movementSpeed);
	velocity = glm::clamp(velocity, -maxVelocity, maxVelocity);

	/*if (camera->type == CAMERA_TYPE_DEBUG) {
		if (glfwGetKey(window, GLFW_KEY_W)) {
			position += camera->front * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			position -= camera->front * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			position -= camera->right * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			position += camera->right * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			position += camera->worldUp * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
			position -= camera->worldUp * speed;
		}
		noclip = true;
	}*/
	if (camera->type == CAMERA_TYPE_FIRST_PERSON) {
		if (window.keyboard.keys[GLFW_KEY_W].down) {
			position += this->front * speed;
		}
		if (window.keyboard.keys[GLFW_KEY_S].down) {
			position -= this->front * speed;
		}
		if (window.keyboard.keys[GLFW_KEY_A].down) {
			position -= camera->right * speed;
		}
		if (window.keyboard.keys[GLFW_KEY_D].down) {
			position += camera->right * speed;
		}

		if (window.keyboard.keys[GLFW_KEY_SPACE].down && grounded) {
			this->velocity.y += jumpForce;
			grounded = false;
		}
		noclip = false;
	}
}

void Player::PollMouseMovement(bool mouseHidden, GLboolean constrainPitch) {
	if (mouseHidden) {
		
		float xpos = window.mouse.postion.x;
		float ypos = window.mouse.postion.y;

		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		camera->pitch += yoffset;	// up/down
		camera->yaw += xoffset;		// left/right

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
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
		RayPoint point = FindPointInDirection(position, camera->front, state.GetMeshes());
	}
}

void Player::PollCollision(float deltatime) {
	bool isGrounded = grounded;
	if (!noclip) {
		std::vector<glm::vec4> normals = PollEachMeshesDistances(state.GetMeshes(), position);
		// checks for any collisions between the environment and the players feet (position)
		for (const glm::vec4& normal : normals) {
			if (normal.w < collisionThreshold) {
				float surfaceDot = glm::dot(glm::normalize(glm::vec3(normal)), this->camera->worldUp);
				//velocity = glm::3reflect(velocity, triangle.normal); // leaving here cause funny
				if (std::abs(surfaceDot) > 0.70f) {
					isGrounded = true;
					velocity.y = 0.0f;
				}
				ResolveCollision(normal, deltatime);
			}
		}
		normals = PollEachMeshesDistances(state.GetMeshes(), position + glm::vec3(0.0f, 2.0f, 0.0f));
		// checks for collisions between the environment and the players head (position + 2)
		for (const glm::vec4& normal : normals) {
			if (normal.w < collisionThreshold) {
				ResolveCollision(normal, deltatime);
			}
		}
		grounded = isGrounded;
	}
}

void Player::ResolveCollision(glm::vec4 normal, float deltatime) {
	// the dot between the triangle and the world up
	float surfaceDot = glm::dot(glm::normalize(glm::vec3(normal)), this->camera->worldUp);
	if (surfaceDot == 0) {
	}
	// should be only slanted walls/floors
	if (surfaceDot != 0 && surfaceDot != 1 && surfaceDot != -1) {
		if (surfaceDot > 0)
			normal = glm::vec4(0, normal.y, 0, normal.w);
		if (surfaceDot < 0)
			normal = glm::vec4(normal.x, 0, normal.z, normal.w);
		glm::normalize(normal);
	}
	//std::cout << triangle.normal.x << " " << triangle.normal.y << " " << triangle.normal.z << "\n";
	float depth = collisionThreshold - normal.w;
	glm::vec3 targetPosition = glm::vec3(normal) * glm::vec3(depth);
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
	PollCollision(tickRate);
	// player movement
	PollMovement(tickRate);
	// mouse look
	PollMouseMovement(true);

	if (glm::abs(velocity.x) >= 0.15f || glm::abs(velocity.y) >= 0.25f || glm::abs(velocity.z) >= 0.15f) {
		moving = true;
	}
	else {
		moving = false;
	}
	// calculate velocity in x and y axis'
	velocity.x += position.x - lastPosition.x; velocity.x = std::clamp(velocity.x, -1.0f, 1.0f); velocity.x = std::lerp(velocity.x, 0.0f, deltatime * 5.0f);
	velocity.z += position.z - lastPosition.z; velocity.z = std::clamp(velocity.z, -1.0f, 1.0f); velocity.z = std::lerp(velocity.z, 0.0f, deltatime * 5.0f);
	// apply transforms
	this->velocity += gravity * deltatime;
	this->position += velocity * deltatime;
	lastPosition = position;
}

bool Player::IsGrounded() const {
	return grounded;
}

bool Player::IsMoving() const {
	return moving;
}

glm::vec3 Player::GetFront() const {
	return front;
}

void Player::SetFront(glm::vec3 front) {
	this->front = front;
}