#pragma once
#include <string>

namespace bf {
	class FileLoader {
	public:
		static bool loadText(const char *path, std::string &result);
	};
}