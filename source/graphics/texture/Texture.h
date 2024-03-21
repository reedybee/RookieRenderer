#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image/stb_image.h>

class Texture {
public:
	// constructor
	Texture();
	// creates the data required to send a texture to the gpu
	Texture(const char* filepath, GLenum format, GLenum wrap = GL_REPEAT, GLenum filter = GL_LINEAR);
	// binds the texture to target for use in shaders
	void Bind(GLenum target);
	// unbinds the texture
	void Unbind();
private:
	unsigned int texture;

	int width;
	int height;
	int nrChannels;

	unsigned char* data;
};