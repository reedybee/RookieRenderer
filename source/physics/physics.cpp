#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh/Mesh.h"
#include "player/Player.h"
#include "physics/Physics.h"
#include "util/util.h"

PhysicsManager::PhysicsManager() {
	gravity = 9.8f;
}

void PhysicsManager::AddMesh(Mesh* mesh) {
	meshes.push_back(mesh);
}

std::vector<DistTriangle> PhysicsManager::PollDistances(glm::vec3 position) {
	std::vector<DistTriangle> distances;
	for (Mesh* mesh : meshes) {
		std::vector<DistTriangle> meshDistances = mesh->GetDistances(position);
		for (const DistTriangle& distance : meshDistances) {
			DistTriangle tri = DistTriangle();
			tri.distance = distance.distance;
			tri.normal = distance.normal;
			tri.tag = mesh->tag;
			tri.enemy = mesh->GetEnemy();
			distances.push_back(tri);
		}
	}
	return distances;
}

Ray PhysicsManager::FindPointDirection(glm::vec3 position, glm::vec3 direction) {
	glm::vec3 lastPos(position);
	glm::vec3 currentPos(position);
	Ray ray = Ray();
	
	int count = 0;
	int maxCount = 100;
	float threshold = 0.01f;
	float distance = 1.0f;
	while (distance >= threshold) {
		count++;
		std::vector<DistTriangle> triangles = PollDistances(lastPos);
		distance = std::numeric_limits<float>::max();
		// gets the closest distances to the current ray pos
		for (unsigned int i = 0; i < triangles.size(); i++) {
			if (triangles[i].distance < distance) {
				distance = triangles[i].distance;
				ray.tag = triangles[i].tag;
				ray.enemy = triangles[i].enemy;
			}
		}
		// breaks function if ray pos is invalid, ex. looking into the sky where there is no valid point
		if (distance > 50000 || distance >= std::numeric_limits<float>::max()) {
			printf("Could not find point on mesh, returning zero!\n");
			return Ray();
			break;
		}
		// this is the final check if the raypos is found.
		if (distance <= threshold)
			break;
		// moves ray pos along rotation
		currentPos = lastPos + direction * distance;
		lastPos = currentPos;
	}
	ray.position = currentPos;
	return ray;
}