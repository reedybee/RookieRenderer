#pragma once
#include <iostream>
#include <vector>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <mesh/Mesh.h>
#include <camera/Camera.h>

struct Enemy {
public:
	// default constuctor
	Enemy();
	// Nondefault constructor
	Enemy(glm::vec3 pos, glm::vec3 rot, const char* meshpath, const char* vertexshader, const char* fragmentshader, Camera* camera);

	void Update();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	float health;
	float maxHealth;

	Mesh mesh;
	Camera* camera;

private:
};

static std::vector<Enemy*> enemies;

static void DrawEnemies(float aspect) {
	for (Enemy* enemy : enemies) {
		enemy->Update();
		enemy->mesh.Draw(aspect);
	}
}