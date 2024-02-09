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

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};


class Mesh {
public:
	Mesh();
	Mesh(const char* filepath);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	void SetData(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);

	void GenerateBuffers();

	std::vector<Vertex> GetVertices();
	std::vector<unsigned int> GetIndices();

	void Draw();
private:
	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;

	unsigned int VAO, VBO, NBO, EBO;

};