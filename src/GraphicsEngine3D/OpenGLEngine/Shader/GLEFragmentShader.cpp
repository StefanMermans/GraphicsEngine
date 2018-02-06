#include "GLEFragmentShader.h"
#include "../../Core/BasicAssetLoader.h"

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

	return compile(data.c_str());
}
