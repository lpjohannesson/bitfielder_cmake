#include "file_loader.h"
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
