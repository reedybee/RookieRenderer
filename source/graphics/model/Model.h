#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader/Shader.h"
#include "camera/Camera.h"
#include "player/Player.h"
#include "mesh/mesh.h"

class Model {
public:
	Model(Camera* camera, const char* vertexFilePath, const char* fragmentFilePath, std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);

	void Draw(float aspect);

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

private:
	Shader shader;
	Mesh mesh;
	Camera* camera;
};