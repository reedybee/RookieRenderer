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
#include "util/util.h"
struct Enemy;
struct Ray {
	glm::vec3 position;
	Enemy* enemy;
	unsigned int tag;
};

struct PhysicsManager {
public:
	PhysicsManager();

	void AddMesh(Mesh* mesh);
	//returns an array of each triangles distances and normal
	std::vector<DistTriangle> PollDistances(glm::vec3 position);
	// finds the closest point on the mesh in the given direction and position
	Ray FindPointDirection(glm::vec3 position, glm::vec3 direction);

	float gravity;
private:
	std::vector<Mesh*> meshes;
};