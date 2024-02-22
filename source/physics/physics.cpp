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
#include "util/util.h"

PhysicsManager::PhysicsManager() {
	gravity = 9.8f;
}

void PhysicsManager::AddMesh(Mesh mesh) {
	meshes.push_back(mesh);
}

std::vector<DistTriangle> PhysicsManager::PollDistances(glm::vec3 position) {
	std::vector<DistTriangle> distances;
	for (Mesh mesh : meshes) {
		std::vector<DistTriangle> meshDistances = mesh.GetDistances(position);
		for (DistTriangle distance : meshDistances) {
			DistTriangle tri = DistTriangle();
			tri.distance = distance.distance;
			tri.normal = distance.normal;
			distances.push_back(tri);
		}
	}
	return distances;
}