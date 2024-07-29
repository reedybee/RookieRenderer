#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>

// reads contents of a file into a string.
static std::string ReadFromFile(const char* folderpath) {
	std::string contents;
	std::ifstream file(folderpath);

	std::string line;

	if (!file.good()) {
		std::cout << "Failed to open file " << folderpath << "\n";
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

// returns a vector of filepaths of an extension from a folder
static std::vector<std::string> GetFilesOfExtFromFolder(std::string pathToFolder, std::string ext) {
	// each filepath that matches the extension
	std::vector<std::string> filepaths;
	for (const std::filesystem::directory_entry entry : std::filesystem::directory_iterator(pathToFolder)) {
		if (entry.is_regular_file()) {
			if (entry.path().extension() == ext) {
				filepaths.push_back(entry.path().string());
			}
		}
	}
	return filepaths;
}