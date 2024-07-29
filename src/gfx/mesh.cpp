#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gfx/texture.h"
#include "gfx/mesh.h"
#include "util/util.h"
#include "player/Player.h"
#include "gfx/window.h"

#include "game/state.h"

Mesh::Mesh() {
	this->colour = glm::vec3(1.0f);
	this->folderpath = nullptr;
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(0.0f);
}
// for imported meshes
Mesh::Mesh(const char* folderpath) {
	this->folderpath = folderpath;
	this->scale = glm::vec3(1.0f);
	this->colour = glm::vec3(1.0f);
	// add this mesh to the players meshes !

	LoadModel();
}

void Mesh::LoadModel() {
	Assimp::Importer importer;
	// finds the path to the object file
	std::string pathToObject;
	std::filesystem::path globalPathToFolder = std::filesystem::current_path().append(folderpath);
	// returns the first object file from the folder specified
	pathToObject = GetFilesOfExtFromFolder(globalPathToFolder.string(), ".obj")[0];
	// opens object
	const aiScene* object = importer.ReadFile(pathToObject, aiProcess_Triangulate | aiProcess_FlipUVs);
	// when the object fails to load, either an incorrect path or corrupt file.
	if (!object || object->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !object->mRootNode)
	{
		std::cout << "Failed to load file " << folderpath << "\n";
		return;
	}
	// loads each submesh
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
				std::string pathToTexture = globalPathToFolder.string().append("/").append(texturepath.C_Str());

				int format = 0;
				std::string ext = std::filesystem::path(pathToTexture).extension().string();
				if (ext == ".png" || ext == ".PNG") {
					format = GL_RGBA;
				} else if (ext == ".jpg" || ext == ".JPG") {
					format = GL_RGB;
				} else {
					printf("Format not recognized: %s\n", ext.c_str());
				}

				submesh.hasTexture = true;
				submesh.texture = Texture(pathToTexture.c_str(), format);
				std::cout << "Loaded texture from " << texturepath.C_Str() << "\n";
			}
		}
		submesh.GenerateBuffers();
		meshes.push_back(submesh);
	}
	std::cout << "Loaded object from: " << pathToObject << "\n";
	importer.FreeScene();
	state.AddMesh(this);
}

void Mesh::Draw() {
	shader.use();
	// setups each matrix from the camera
	glm::mat4 projection = state.GetPlayer()->camera->GetProjectionMatrix((float)window.resolution.x / (float)window.resolution.y);
	glm::mat4 view = state.GetPlayer()->camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	// sends data to shader
	shader.SetVector3("cameraPos", state.GetPlayer()->camera->position);
	shader.SetVector3("lightPosition", state.GetPlayer()->camera->lightPosition);
	shader.SetVector3("objectColour", colour);
	shader.SetMatrix4("projection", projection);
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("model", model);
	// draws each sub meshes mesh
	for (Submesh mesh : meshes) {
		if (mesh.hasTexture) {
			shader.SetInt("albedoMap", 0);
			mesh.texture.Bind(GL_TEXTURE0);
		}
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh.indices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		mesh.texture.Unbind();
	}
}

std::vector<glm::vec4> Mesh::GetDistances(glm::vec3 position) {
	std::vector<glm::vec4> normals = std::vector<glm::vec4>();
	glm::vec4 normal = glm::vec4();
	for (int s = 0; s < meshes.size(); s++) {
		for (int i = 0; i < meshes[s].vertices.size(); i += 3) {
			// fun function
			glm::vec3 v1 = meshes[s].vertices[i].Position;
			glm::vec3 v2 = meshes[s].vertices[i + 1].Position;
			glm::vec3 v3 = meshes[s].vertices[i + 2].Position;

			glm::vec3 s1 = ((v1 + this->position) * this->scale);
			glm::vec3 s2 = ((v2 + this->position) * this->scale);
			glm::vec3 s3 = ((v3 + this->position) * this->scale);

			glm::vec3 n1 = meshes[s].vertices[i].Normal;
			glm::vec3 n2 = meshes[s].vertices[i + 1].Normal;
			glm::vec3 n3 = meshes[s].vertices[i + 2].Normal;
			glm::vec3 norm = glm::normalize(n1 + n2 + n3);
			float distance = sdfTriangle(position, s1, s2, s3);

			normal.x = norm.x;
			normal.y = norm.y;
			normal.z = norm.z;
			normal.w = distance;
			normals.push_back(normal);
		}
	}
	return normals;
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
	// copies data to buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture co-ords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// element buffer
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}