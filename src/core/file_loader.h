#pragma once
#include <vector>
#include <string>

namespace bf {
	class FileLoader {
	public:
		static bool loadText(const char *path, std::string &result);

		static void getFilePaths(const char *basePath, std::vector<std::string> &result);
		static void getDirectoryNames(const char *basePath, std::vector<std::string> &result);
	};
}