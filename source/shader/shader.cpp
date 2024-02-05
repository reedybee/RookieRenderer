#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>

#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	std::string currentLine;

	std::string vertexCode;
	std::ifstream vertexFile = std::ifstream(vertexShaderPath);
	if (vertexFile.is_open()) {
		while (std::getline(vertexFile, currentLine)) {
			currentLine.append("\n");
			vertexCode.append(currentLine);
			std::cout << currentLine;
		}
	}
	vertexFile.close();
}