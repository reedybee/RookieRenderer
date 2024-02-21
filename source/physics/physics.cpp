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

PhysicsManager::PhysicsManager() {
}

void PhysicsManager::AddMesh(Mesh mesh) {
	meshes.push_back(mesh);
}

std::vector<PhysicsTriangle> PhysicsManager::PollDistances(glm::vec3 position) {
	glm::vec3 normal = glm::vec3(0.0f);
	std::vector<PhysicsTriangle> distances;
	for (Mesh mesh : meshes) {
		std::vector<float> meshDistances = mesh.GetDistances(position, normal);
		for (float distance : meshDistances) {
			PhysicsTriangle tri = PhysicsTriangle();
			tri.distance = distance;
			tri.normal = normal;
			distances.push_back(tri);
		}
	}
	return distances;
}