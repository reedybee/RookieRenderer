#include <iostream>
#include <vector>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "player/Enemy.h"

Enemy::Enemy() {
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);

	health = 100;
	maxHealth = 100;

	mesh = Mesh();
	camera = new Camera();
}

Enemy::Enemy(glm::vec3 pos, glm::vec3 rot, const char* meshpath, const char* vertexshader, const char* fragmentshader, Camera* camera) {
	position = pos;
	rotation = rot;
	scale = glm::vec3(1.0f);

	health = 100;
	maxHealth = 100;

	this->camera = camera;
	mesh = Mesh(meshpath, this->camera);
	mesh.shader = Shader(vertexshader, fragmentshader);
	mesh.tag = MESH_ENEMY;
}

void Enemy::Update() {
	mesh.position = position;
	mesh.rotation = rotation;
	mesh.scale = scale;
}