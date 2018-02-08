#include "GLEVertexShader.h"
#include "../../Core/BasicAssetLoader.h"
#include "../../Core/Logger.h"

using Core::Logger;

GLEVertexShader::GLEVertexShader(const std::string &filePath) :
	GLEShader(filePath, GL_VERTEX_SHADER)
{
}

GLEVertexShader::GLEVertexShader() :
	GLEShader(GL_VERTEX_SHADER)
{
}

bool GLEVertexShader::init()
{
	std::string data =
		Core::BasicAssetLoader::readFile(_filePath);

	if (!compile(data.c_str())) {
		Logger::logLine("Vertex shader");
		return false;
	}
	else {
		return true;
	}
}

bool GLEVertexShader::attach(const GLuint & programId)
{
	if (GLEShader::attach(programId)) {
		return true;
	}

	return false;
}
