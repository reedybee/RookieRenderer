#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh/Mesh.h"

Mesh::Mesh() {
	this->vertices = std::vector<Vertex>();
	this->indices = std::vector<unsigned int>();
}

Mesh::Mesh(const char* filepath) {
	this->vertices = std::vector<Vertex>();
	this->indices = std::vector<unsigned int>();

	Assimp::Importer importer;

	const aiScene* object = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

	// assuming its a single object
	aiMesh* mesh = object->mMeshes[0];

	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex = Vertex();
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->HasNormals()) {
			vertex.Normal = glm::normalize(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
		}
		vertices.push_back(vertex);
	}
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int f = 0; f < face.mNumIndices; f++) {
			indices.push_back(face.mIndices[f]);
		}
	}
	importer.FreeScene();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
	this->vertices = vertices;
	this->indices = indices;

	GenerateBuffers();
}

void Mesh::SetData(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices) {
	//this->vertices = vertices;
	this->indices = indices;
}


std::vector<Vertex> Mesh::GetVertices() {
	return vertices;
}

std::vector<unsigned int> Mesh::GetIndices() {
	return indices;
}

void Mesh::Draw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void Mesh::GenerateBuffers() {
	// create arrays
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// copys data to buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// element buffer
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}