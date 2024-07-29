#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gfx/camera.h"
#include "util/util.h"

#include "Shader.h"

Shader::Shader() {
	program = NULL;
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	// where to store vertex shader contents
	std::string vertexCode = ReadFromFile(vertexShaderPath);
	// creates and compiles vertex shader
	const char* vertexCodeChar = vertexCode.c_str();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCodeChar, NULL);
	glCompileShader(vertexShader);
	
	// where to store fragment shader contents
	std::string fragmentCode = ReadFromFile(fragmentShaderPath);
	// creates and compiles fragment shader
	const char* fragmentCodeChar = fragmentCode.c_str();
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCodeChar, NULL);
	glCompileShader(fragmentShader);
	// create and attaches shader to program
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glUseProgram(program); 
}

void Shader::use() {
	glUseProgram(program);
}

#pragma region SETTERS

void Shader::SetInt(const char* name, int value) const {
	glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::SetFloat(const char* name, float value) const {
	glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::SetVector2(const char* name, float x, float y) const {
	glUniform2f(glGetUniformLocation(program, name), x, y);
}

void Shader::SetVector2(const char* name, glm::vec2 vector) const {
	glUniform2f(glGetUniformLocation(program, name), vector.x, vector.y);
}

void Shader::SetVector3(const char* name, glm::vec3 vector) const {
	glUniform3f(glGetUniformLocation(program, name), vector.x, vector.y, vector.z);
}

void Shader::SetVector3(const char* name, int vector[]) const {
	glUniform3i(glGetUniformLocation(program, name), vector[0], vector[1], vector[2]);
}

void Shader::SetVector3(const char* name, float vector[]) const {
	glUniform3f(glGetUniformLocation(program, name), vector[0], vector[1], vector[2]);
}

void Shader::SetVector3(const char* name, int x, int y, int z) const {
	glUniform3i(glGetUniformLocation(program, name), x, y, z);
}

void Shader::SetVector4(const char* name, glm::vec4 vector) {
	glUniform4f(glGetUniformLocation(program, name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetMatrix4(const char* name, glm::mat4 matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

#pragma endregion
