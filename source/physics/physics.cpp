#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh/Mesh.h"
#include "physics/Physics.h"

PhysicsManager::PhysicsManager(Player* player) {
	this->player = player;
}

void PhysicsManager::AddMesh(Mesh mesh) {
	meshes.push_back(mesh);
}

std::vector<float> PhysicsManager::PollDistances() {
	glm::vec3 normal = glm::vec3(0.0f);
	std::vector<float> distances;
	for (Mesh mesh : meshes) {
		std::vector<float> meshDistances = mesh.GetDistances(player->position, normal);
		for (float distance : meshDistances) {
			distances.push_back(distance);
		}
	}
	return distances;
}