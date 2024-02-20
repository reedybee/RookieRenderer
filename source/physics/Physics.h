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
#include "player/Player.h"

struct PhysicsManager {
public:
	PhysicsManager(Player* player);

	void AddMesh(Mesh mesh);
	std::vector<float> PollDistances();
private:
	std::vector<Mesh> meshes;
	Player* player;
};