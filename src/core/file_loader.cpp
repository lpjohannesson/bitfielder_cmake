#include "core/file_loader.h"
#include <fstream>
#include <sstream>

using namespace bf;

bool FileLoader::loadText(const std::string path, std::string &result) {
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

void FileLoader::getFilePathObjects(const std::string basePath, std::vector<std::filesystem::path> &result) {
	for (auto &entry : std::filesystem::recursive_directory_iterator(basePath)) {
		if (entry.is_directory()) {
			continue;
		}

		result.push_back(entry.path());
	}
}

void FileLoader::getFilePaths(const std::string basePath, std::vector<std::string> &result) {
	std::vector<std::filesystem::path> filePaths;
	getFilePathObjects(basePath, filePaths);
	
	for (const auto &filePath : filePaths) {
		result.push_back(filePath.string());
	}
}

void FileLoader::getDirectoryNames(const std::string basePath, std::vector<std::string> &result) {
	for (auto &entry : std::filesystem::directory_iterator(basePath)) {
		if (!entry.is_directory()) {
			continue;
		}

		result.push_back(entry.path().filename().string());
	}
}