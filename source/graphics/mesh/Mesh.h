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
	Mesh();
	Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);

	void SetData(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);

	void ReadFromFile(const char* filepath);

	std::vector<glm::vec3> GetVertices();
	std::vector<unsigned int> GetIndices();

	void Draw();
private:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec2> textureCoords;
	std::vector<unsigned int> textureIndices;
	std::vector<glm::vec3> inputNormals;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> normalIndices;

	unsigned int VAO, VBO, TBO, NBO, EBO;

	void GenerateBuffers();
};