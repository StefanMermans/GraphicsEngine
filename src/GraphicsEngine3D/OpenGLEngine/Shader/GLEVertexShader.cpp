#include "GLEVertexShader.h"
#include "../../Core/BasicAssetLoader.h"

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

	bool success = compile(data.c_str());

	// TODO: extra stuff here?

	return success;
}

bool GLEVertexShader::attach(const GLuint & programId)
{
	if (GLEShader::attach(programId)) {
		// We use vertex attribute 0 and 1. whatever that means.
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		return true;
	}

	return false;
}
