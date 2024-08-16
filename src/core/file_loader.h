#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <rapidjson/document.h>

namespace bf {
	class FileLoader {
	public:
		static bool loadText(const std::string path, std::string &result);
		static bool loadJson(const std::string path, rapidjson::Document &result);

		static void getFilePathObjects(const std::string basePath, std::vector<std::filesystem::path> &result);
		static void getFilePaths(const std::string basePath, std::vector<std::string> &result);
		
		static void getDirectoryNames(const std::string basePath, std::vector<std::string> &result);
	};
}