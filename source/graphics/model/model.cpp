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

#include "model/Model.h"

Model::Model(Camera* camera, const char* vertexFilePath, const char* fragmentFilePath, Mesh mesh) {
	shader = Shader(vertexFilePath, fragmentFilePath);
	this->mesh = mesh;
	this->camera = camera;
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotation = glm::vec3(0.0f);
}


Model::Model(Camera* camera, const char* vertexFilePath, const char* fragmentFilePath, std::vector<glm::vec3> vertices, std::vector<unsigned int> indices) {
	shader = Shader(vertexFilePath, fragmentFilePath);
	mesh = Mesh(vertices, indices);
	this->camera = camera;
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotation = glm::vec3(0.0f);
}

void Model::Draw(float aspect) {
	shader.use();

	glm::mat4 projection = camera->GetProjectionMatrix(aspect);

	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, scale);
	model = glm::translate(model, position);

	shader.SetMatrix4("projection", projection);
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("model", model);

	mesh.Draw();
}