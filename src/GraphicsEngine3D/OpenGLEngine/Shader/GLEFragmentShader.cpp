#include "GLEFragmentShader.h"
#include "../../Core/BasicAssetLoader.h"
#include "../../Core/Logger.h"

using Core::Logger;

GLEFragmentShader::GLEFragmentShader(const std::string &filePath)
	: GLEShader(filePath, GL_FRAGMENT_SHADER)
{
}

GLEFragmentShader::GLEFragmentShader() :
	GLEShader(GL_FRAGMENT_SHADER)
{
}

bool GLEFragmentShader::init()
{
	std::string data =
		Core::BasicAssetLoader::readFile(_filePath);

	if (!compile(data.c_str())) {
		Logger::logLine("Fragment shader");
		return false;
	}
	else {
		return true;
	}
}
