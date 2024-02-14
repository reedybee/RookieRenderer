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

#include "texture/Texture.h"
#include "mesh/Mesh.h"

Mesh::Mesh(const char* filepath, Camera* camera) {
	this->vertices = std::vector<Vertex>();
	this->indices = std::vector<unsigned int>();
	this->filepath = filepath;
	this->camera = camera;

	this->scale = glm::vec3(1.0f);

	LoadModel();
}

void Mesh::LoadModel() {
	this->vertices = std::vector<Vertex>();
	this->indices = std::vector<unsigned int>();

	Assimp::Importer importer;

	const aiScene* object = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!object || object->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !object->mRootNode)
	{
		std::cout << "Failed to load file " << filepath << "\n";
		return;
	}

	// assuming its a single object
	aiMesh* mesh = object->mMeshes[0];

	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex = Vertex();
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->HasNormals()) {
			vertex.Normal = glm::normalize(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
		}
		if (mesh->HasTextureCoords(0)) {
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;

			if (mesh->HasTangentsAndBitangents()) {
				vertex.Tangent.x = mesh->mTangents[i].x;
				vertex.Tangent.y = mesh->mTangents[i].y;
				vertex.Tangent.z = mesh->mTangents[i].z;

				vertex.Bitangent.x = mesh->mBitangents[i].x;
				vertex.Bitangent.y = mesh->mBitangents[i].y;
				vertex.Bitangent.z = mesh->mBitangents[i].z;
			}
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
	this->GenerateBuffers();
}

std::vector<Vertex> Mesh::GetVertices() {
	return vertices;
}

std::vector<unsigned int> Mesh::GetIndices() {
	return indices;
}

void Mesh::Draw(float aspect) {
	shader.use();

	shader.SetInt("albedoMap", 0);
	shader.SetInt("roughnessMap", 1);
	shader.SetInt("normalMap", 2);

	glm::mat4 projection = camera->GetProjectionMatrix(aspect);

	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, scale);
	model = glm::translate(model, position);

	shader.SetVector3("cameraPos", camera->position);
	shader.SetVector3("lightPosition", camera->lightPosition);
	shader.SetVector3("objectColour", colour);
	shader.SetMatrix4("projection", projection);
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("model", model);

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
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// element buffer
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}