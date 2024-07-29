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

#include "gfx/shader.h"
#include "gfx/camera.h"
#include "gfx/texture.h"
#include "gfx/window.h"
#include "util/util.h"
class Player;

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

// a sub mesh: uses separate textures
struct Submesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Texture texture;

	void GenerateBuffers();

	bool hasTexture = false;
	unsigned int VAO = NULL, VBO = NULL, NBO = NULL, EBO = NULL;
};

// one total mesh
class Mesh {
public:
	Mesh();
	Mesh(const char* folderpath);
	// renders the mesh
	void Draw();
	// returns an array of vec4's where x, y, z is the normal of the triangle and w is the closest distance of each triangle in the mesh
	std::vector<glm::vec4> GetDistances(glm::vec3 position);
	// returns the number of triangles the make up the mesh
	unsigned int GetNumTriangles();
	// 
	std::vector<Submesh> GetSubmeshes();

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 colour;
	Shader shader;
private:
	void LoadModel();

	bool ranOnce = false;
	const char* folderpath;
	std::vector<glm::vec3> vertices;
	std::vector<Submesh> meshes;
};

// returns array of each distance to each triangle in an array of meshes from some postion.
static std::vector<glm::vec4> PollEachMeshesDistances(std::vector<Mesh*> meshes, glm::vec3 position) {
	std::vector<glm::vec4> normals;
	for (Mesh* mesh : meshes) {
		std::vector<glm::vec4> meshDistances = mesh->GetDistances(position);
		for (glm::vec4 normal : meshDistances) {
			normals.push_back(normal);
		}
	}
	return normals;
}

// finds the closest point on the mesh in the given direction and position
static RayPoint FindPointInDirection(glm::vec3 position, glm::vec3 direction, std::vector<Mesh*> meshes) {
	glm::vec3 lastPos(position);
	glm::vec3 currentPos(position);
	RayPoint point = RayPoint();

	float threshold = 0.01f;
	float distance = std::numeric_limits<float>::max();
	while (distance >= threshold) {
		std::vector<glm::vec4> normals = PollEachMeshesDistances(meshes, lastPos);
		for (const glm::vec4& normal : normals) {
			if (normal.w < distance) {
				distance = normal.w;
			}
		}
		if (distance > 50000) {
			printf("Could not find point on mesh, returning zero!\n");
			return RayPoint();
		}
		if (distance <= threshold) {
			break;
		}
		currentPos = lastPos + direction * distance;
		lastPos = currentPos;
	}
	point.position = lastPos;
	return point;
}