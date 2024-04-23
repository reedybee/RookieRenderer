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
#include "util/util.h"
class Player;
// uses an unsigned 8 bit number
enum MESH_TAG {
	MESH_NONE			= 0b00000001,
	MESH_ENVIRONMENT	= 0b00000010,
	MESH_LOCAL_PLAYER	= 0b00000100,
	MESH_NETWORK_PLAYER	= 0b00001000,
	MESH_ENEMY			= 0b00010000,
	MESH_COLLIDER		= 0b00100000,
	MESH_EMPTY2			= 0b01000000,
	MESH_EMPTY3			= 0b10000000,
};

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

// a submesh: uses separate textures
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
	Mesh(const char* filepath, Player* player);

	void Draw(float aspect);
	// returns the distances to each triangle in the mesh
	std::vector<DistTriangle> GetDistances(glm::vec3 position);
	// returns the number of triangles the make up the mesh
	unsigned int GetNumTriangles();
	// 
	std::vector<Submesh> GetSubmeshes();

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 colour;
	Shader shader; 
	unsigned char tag;
private:
	void LoadModel();

	Player* player;
	bool ranOnce = false;
	const char* filepath;
	std::vector<glm::vec3> vertices;
	std::vector<Submesh> meshes;
};

// returns array of each distance to each triangle in an array of meshes from some postion.
static std::vector<DistTriangle> PollEachMeshesDistances(std::vector<Mesh*> meshes, glm::vec3 position) {
	std::vector<DistTriangle> distances;
	for (Mesh* mesh : meshes) {
		std::vector<DistTriangle> meshDistances = mesh->GetDistances(position);
		for (DistTriangle tri : meshDistances) {
			tri.tag = mesh->tag;
			distances.push_back(tri);
		}
	}
	return distances;
}

// finds the closest point on the mesh in the given direction and position
static RayPoint FindPointInDirection(glm::vec3 position, glm::vec3 direction, std::vector<Mesh*> meshes) {
	glm::vec3 lastPos(position);
	glm::vec3 currentPos(position);
	RayPoint point;

	float threshold = 0.01f;
	float distance = std::numeric_limits<float>::max();
	while (distance >= threshold) {
		std::vector<DistTriangle> triangles = PollEachMeshesDistances(meshes, lastPos);
		for (const DistTriangle& tri : triangles) {
			if (tri.distance < distance) {
				distance = tri.distance;
				point.tag = tri.tag;
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