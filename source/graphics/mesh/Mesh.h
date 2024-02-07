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

class Mesh {
public:
	Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);

	void Draw();
private:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO, VBO, EBO;
};