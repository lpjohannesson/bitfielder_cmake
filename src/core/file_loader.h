#pragma once
#include <vector>
#include <string>
#include <filesystem>

namespace bf {
	class FileLoader {
	public:
		static bool loadText(const std::string path, std::string &result);

		static void getFilePathObjects(const std::string basePath, std::vector<std::filesystem::path> &result);
		static void getFilePaths(const std::string basePath, std::vector<std::string> &result);
		
		static void getDirectoryNames(const std::string basePath, std::vector<std::string> &result);
	};
}