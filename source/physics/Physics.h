#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh/Mesh.h"

struct PhysicsTriangle {
	float distance;
	glm::vec3 normal;
};

struct PhysicsManager {
public:
	PhysicsManager();

	void AddMesh(Mesh mesh);
	//returns an array of each triangles distances and normal
	std::vector<PhysicsTriangle> PollDistances(glm::vec3 position);
private:
	std::vector<Mesh> meshes;
};