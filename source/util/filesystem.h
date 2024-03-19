#pragma once
#include <iostream>
#include <fstream>
#include <string>

// reads contents of a file into a string.
static std::string ReadFromFile(const char* filepath) {
	std::string contents;
	std::ifstream file(filepath);

	std::string line;

	if (!file.good()) {
		std::cout << "Failed to open file " << filepath << "\n";
	}
	if (file.is_open()) {
		while (std::getline(file, line)) {
			line.append("\n");
			contents.append(line);
		}
	}
	file.close();

	return contents;
}
