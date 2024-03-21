#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image/stb_image.h>

#include "Texture.h"

Texture::Texture() {
	data = NULL;
	texture = NULL;
	width = NULL;
	height = NULL;
	nrChannels = NULL;
}

Texture::Texture(const char* filepath, GLenum format, GLenum wrap, GLenum filter) {

	data = stbi_load(filepath, &width, &height, &nrChannels, 0);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (!data) {
		std::cout << "Image Failed to Load: " << stbi_failure_reason << "\n";
	}

	stbi_image_free(data);
}

void Texture::Bind(GLenum target) {
	glActiveTexture(target);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}