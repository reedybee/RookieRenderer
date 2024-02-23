#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image/stb_image.h>

#include "texture/Texture.h"
#include "mesh/Mesh.h"
#include "camera/Camera.h"

struct Sprite {
public:
	Sprite(glm::vec3 position, glm::vec3 scale, bool billboard, Camera* camera);

	void Draw(float aspect);

	glm::vec3 position, scale, rotation;
	bool billboard;
private:
	glm::vec3 front;
	Mesh mesh;
	Camera* camera;
};