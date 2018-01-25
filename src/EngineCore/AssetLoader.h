#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <string>

namespace Core {
	class AssetLoader
	{
	public:
		static void setRootPath(const std::string &rootPath);
		static std::string getRootPath();
	private:
		static std::string _rootPath;
	};
};

#endif // !ASSET_LOADER_H
