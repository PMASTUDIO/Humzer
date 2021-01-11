#pragma once

#include <string>

namespace Humzer {

	class FileDialogs {
	public:
		// Returns empty string if canceled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}
