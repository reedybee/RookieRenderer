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
#include "texture/Texture.h"

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

struct Submesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Texture texture;

	void GenerateBuffers();

	unsigned int VAO, VBO, NBO, EBO;
};


class Mesh {
public:
	Mesh(const char* filepath, Camera* camera);

	void Draw(float aspect);

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 colour;

	Shader shader; 
private:
	void LoadModel();

	Camera* camera;

	const char* filepath;

	std::vector<Submesh> meshes;
};