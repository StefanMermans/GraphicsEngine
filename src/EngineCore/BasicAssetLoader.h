#ifndef BASIC_ASSET_LOADER_H
#define BASIC_ASSET_LOADER_H

#include <string>
#include <iostream>
#include <fstream>

namespace Core{
	// The basic asset loader really just opens files...
	class BasicAssetLoader
	{
	public:
		static bool readFile(const std::string &path, std::string &buffer);

		static std::ifstream loadStream(const std::string &path, bool &succes);
		static std::ifstream loadStream(const std::string &path);
	};
}

#endif
