#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image/stb_image.h>

#include "sprite/Sprite.h"

#include "texture/Texture.h"
#include "mesh/Mesh.h"
#include "camera/Camera.h"

Sprite::Sprite(glm::vec3 position, glm::vec3 scale, bool billboard, Camera* camera) {
	this->position = position;
	this->scale = scale;
	this->rotation = glm::vec3(0.0f);
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->billboard = billboard;
	this->camera = camera;
	this->mesh = Mesh("resources/objects/sprite.obj", this->camera);
	this->mesh.shader = Shader("resources/shaders/unlit/unlitvertex.glsl", "resources/shaders/unlit/unlitfragment.glsl");
}

void Sprite::Draw(float aspect) {
	mesh.position = position;
	mesh.rotation = rotation;
	mesh.scale = scale;

	mesh.Draw(aspect);
}