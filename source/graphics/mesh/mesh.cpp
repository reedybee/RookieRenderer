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
#include "util/util.h"

Mesh::Mesh() {
	this->camera = nullptr;
	this->colour = glm::vec3(0.0f);
	this->filepath = nullptr;
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(0.0f);
}
// for imported meshes
Mesh::Mesh(const char* filepath, Camera* camera) {
	this->filepath = filepath;
	this->camera = camera;

	this->scale = glm::vec3(1.0f);

	LoadModel();
}

void Mesh::LoadModel() {
	Assimp::Importer importer;

	const aiScene* object = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!object || object->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !object->mRootNode)
	{
		std::cout << "Failed to load file " << filepath << "\n";
		return;
	}

	for (unsigned int m = 0; m < object->mNumMeshes; m++) {
		Submesh submesh;
		aiMesh* mesh = object->mMeshes[m];
		// gets each of the submeshes vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex = Vertex();
			vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			if (mesh->HasNormals()) {
				vertex.Normal = glm::normalize(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
			}
			// gets the submeshes texture coordinates if it contains them
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
			submesh.vertices.push_back(vertex);
		}
		// gets the submeshes indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int f = 0; f < face.mNumIndices; f++) {
				submesh.indices.push_back(face.mIndices[f]);
			}
		}
		// gets the submeshes material
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = object->mMaterials[mesh->mMaterialIndex];
			aiString texturepath;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturepath) == aiReturn_SUCCESS) {
				submesh.hasTexture = true;
				submesh.texture = Texture(texturepath.C_Str(), GL_RGB);
				std::cout << "Loaded texture from " << texturepath.C_Str() << "\n";
			}
		}
		if (submesh.hasTexture) {
			std::cout << "Yes\n";
		}
		if (!submesh.hasTexture) {
			std::cout << "no\n";
		}
		submesh.GenerateBuffers();
		meshes.push_back(submesh);
	}
	importer.FreeScene();
}

void Mesh::Draw(float aspect) {
	shader.use();

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

	for (Submesh mesh : meshes) {
		if (mesh.hasTexture) {
			shader.SetInt("albedoMap", 0);
			mesh.texture.Bind(GL_TEXTURE0);
		}
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh.indices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
}

std::vector<DistTriangle> Mesh::GetDistances(glm::vec3 position) {
	//float lowest = std::numeric_limits<float>::max();
	std::vector<DistTriangle> triangles = std::vector<DistTriangle>();
	DistTriangle triangle = DistTriangle();
	for (int s = 0; s < meshes.size(); s++) {
		for (int i = 0; i < meshes[s].vertices.size(); i += 3) {
			glm::vec3 v1 = meshes[s].vertices[i].Position;
			glm::vec3 v2 = meshes[s].vertices[i + 1].Position;
			glm::vec3 v3 = meshes[s].vertices[i + 2].Position;

			glm::vec3 s1 = ((v1 + this->position) * this->scale);
			glm::vec3 s2 = ((v2 + this->position) * this->scale);
			glm::vec3 s3 = ((v3 + this->position) * this->scale);

			glm::vec3 n1 = meshes[s].vertices[i].Normal;
			glm::vec3 n2 = meshes[s].vertices[i + 1].Normal;
			glm::vec3 n3 = meshes[s].vertices[i + 2].Normal;
			glm::vec3 normal = glm::normalize(n1 + n2 + n3);
			float distance = sdfTriangle(position, s1, s2, s3);

			triangle.distance = distance;
			triangle.normal = normal;
			triangles.push_back(triangle);
		}
	}
	return triangles;
}

unsigned int Mesh::GetNumTriangles() {
	unsigned int numTriangles = 0;
	for (unsigned int meshIndex = 0; meshIndex < meshes.size(); meshIndex++) {
		for (unsigned int index = 0; index < meshes[meshIndex].indices.size(); index++) {
			numTriangles++;
		}
	}
	std::cout << numTriangles / 3 << "\n";
	return numTriangles / 3;
}

std::vector<Submesh> Mesh::GetSubmeshes() {
	return meshes;
}

void Submesh::GenerateBuffers() {
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