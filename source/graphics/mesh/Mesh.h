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
	// returns the distances to each triangle in the mesh,
	// if the threshold value is included ignores any triangles outside the threshold
	std::vector<float> GetDistances(glm::vec3 position, glm::vec3& normal);
	// returns the number of triangles the make up the mesh
	unsigned int GetNumTriangles();

	std::vector<Submesh> GetSubmeshes();

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 colour;

	Shader shader; 
private:
	void LoadModel();

	Camera* camera;
	bool ranOnce = false;
	const char* filepath;

	std::vector<Submesh> meshes;
};