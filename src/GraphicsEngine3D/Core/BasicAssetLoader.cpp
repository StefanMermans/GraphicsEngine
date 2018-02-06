#include "BasicAssetLoader.h"
#include "Logger.h"

namespace Core {
	bool BasicAssetLoader::try_readFile(const std::string &path, std::string &buffer)
	{
		// Open an input stream
		std::ifstream stream = loadStream(path);
		if (!stream.is_open()) {
			return false;
		}

		// Read from the stream
		buffer = std::string{
			(std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>()
		};

		return true;
	}

	std::string BasicAssetLoader::readFile(const std::string & path)
	{
		std::string buffer;
		if (!try_readFile(path, buffer)) {
			Logger::errorLine("Something went wrong while opening file:\n    " + path);
			return std::string{};
		}
		
		return buffer;
	}

	std::ifstream BasicAssetLoader::loadStream(const std::string &path, bool &succes)
	{
		std::ifstream stream = loadStream(path);

		succes = stream.is_open();

		return stream;
	}

	std::ifstream BasicAssetLoader::loadStream(const std::string &path)
	{
		std::ifstream stream(path.c_str());

		return stream;
	}
}
