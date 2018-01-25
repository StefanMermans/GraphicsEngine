#include "BasicAssetLoader.h"

namespace Core {
	bool BasicAssetLoader::readFile(const std::string & path, std::string & buffer)
	{
		std::ifstream stream = loadStream(path);
		if (!stream.is_open()) {
			return false;
		}

		std::string lineBuffer;
		while (std::getline(stream, lineBuffer)) {
			buffer.append(lineBuffer);
		}

		return true;
	}

	std::ifstream BasicAssetLoader::loadStream(const std::string &path, bool &succes)
	{
		std::ifstream stream = loadStream(path);

		succes = stream.is_open();

		return stream;
	}

	std::ifstream BasicAssetLoader::loadStream(const std::string & path)
	{
		std::ifstream stream(path.c_str());

		return stream;
	}
}
