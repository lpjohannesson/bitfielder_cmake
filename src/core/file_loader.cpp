#include "core/file_loader.h"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace bf;

bool FileLoader::loadText(const char *path, std::string &result) {
	std::fstream file;
	file.open(path);

	if (!file.is_open()) {
		return false;
	}

	std::string line;
	std::stringstream stream;

	while (getline(file, line)) {
		stream << line << std::endl;
	}

	file.close();

	result = stream.str();
	return true;
}

void FileLoader::getFilePaths(const char *basePath, std::vector<std::string> &result) {
	for (auto &entry : std::filesystem::recursive_directory_iterator(basePath)) {
		if (entry.is_directory()) {
			continue;
		}

		result.push_back(entry.path().string());
	}
}

void FileLoader::getDirectoryNames(const char *basePath, std::vector<std::string> &result) {
	for (auto &entry : std::filesystem::directory_iterator(basePath)) {
		if (!entry.is_directory()) {
			continue;
		}

		result.push_back(entry.path().filename().string());
	}
}