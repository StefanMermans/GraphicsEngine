#include "AssetLoader.h"

// This file is part of the Core namespace
using namespace Core;

std::string AssetLoader::_rootPath = "";

void AssetLoader::setRootPath(const std::string &rootPath)
{
	_rootPath = rootPath;
}

std::string AssetLoader::getRootPath()
{
	return _rootPath;
}
