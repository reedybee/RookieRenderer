#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh/Mesh.h"

Mesh::Mesh() {
	this->vertices = std::vector<glm::vec3>();
	this->indices = std::vector<unsigned int>();
}

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices) {
	this->vertices = vertices;
	this->indices = indices;

	GenerateBuffers();
}

void Mesh::SetData(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices) {
	this->vertices = vertices;
	this->indices = indices;

	GenerateBuffers();
}


std::vector<glm::vec3> Mesh::GetVertices() {
	return vertices;
}
std::vector<unsigned int> Mesh::GetIndices() {
	return indices;
}

void Mesh::ReadFromFile(const char* filepath) {
	std::ifstream file = std::ifstream(filepath);
	vertices.clear();
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			char lineType;
			iss >> lineType;

			if (lineType == 'v') {
				if (iss.peek() == ' ') {
					glm::vec3 vertex = glm::vec3();
					iss >> vertex.x >> vertex.y >> vertex.z;
					vertices.push_back(glm::fvec3(vertex));
					std::cout << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
				}
				if (iss.peek() == 't') {
					glm::fvec2 texCoord = glm::fvec2();
					iss >> texCoord.x >> texCoord.y;
					textureCoords.push_back((glm::fvec2)texCoord);
					std::cout << texCoord.x << " " << texCoord.y << "\n";
				}
			}

			if (lineType == 'f' && iss.peek() == ' ') {
				char delimeter;
				unsigned int vertexIndex = 0;
				unsigned int textureIndex = 0;
				unsigned int normalIndex = 0;
				for (int i = 0; i < 3; i++) {
					iss >> vertexIndex >> delimeter >> textureIndex >> delimeter >> normalIndex;
					indices.push_back(vertexIndex - 1);
					textureIndices.push_back(textureIndex - 1);
					normalIndices.push_back(normalIndex - 1);
					std::cout << vertexIndex - 1 << " " << textureIndex - 1 << " " << normalIndex - 1 << "\n";
				}
			}
		}
	}
	file.close();
	GenerateBuffers();
}

void Mesh::Draw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void Mesh::GenerateBuffers() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_READ);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);
}