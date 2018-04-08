#include "GLEShader.h"
#include "../../Core/Logger.h"

using Core::Logger;

GLEShader::GLEShader(const std::string & filePath, const GLenum &shaderType) :
	_filePath(filePath)
{
	_id = glCreateShader(shaderType);
	_state = GLE_SHADER_CREATED;
}

GLEShader::GLEShader(const GLenum &shaderType)
{
	_state = GLE_SHADER_CREATED_NO_FILES;
}

GLEShader::~GLEShader()
{
	if (_state != GLE_SHADER_CREATED_NO_FILES) {
		glDeleteShader(_id);
	}
}

GLEShaderState GLEShader::getState() const
{
	return _state;
}

bool GLEShader::attach(const GLuint & programId)
{
	if (_state == GLE_SHADER_COMPILED) {
		glAttachShader(programId, _id);
		return true;
	}
	return false;
}

bool GLEShader::compile(const char * shaderData)
{
	// Read the shader from the shader data.
	glShaderSource(_id, 1, &shaderData, NULL);
	// Compile the shader
	glCompileShader(_id);
	// Check for errors and return.
	if (checkErrors(_id)) {
		_state = GLE_SHADER_COMPILED;
		return true;
	}
	_state = GLE_SHADER_COMPILATION_FAILED;
	return false;
}

bool GLEShader::checkErrors(const GLuint & shaderId)
{
	GLint status;
	// Check the compile status of the shader.
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		int length;
		// get the length of the message.
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* infolog = new char[length + 1];
		memset(infolog, 0, length + 1);
		// get the message itself.
		glGetShaderInfoLog(shaderId, length, NULL, infolog);
		Logger::errorLine(std::string("Error compiling shader:\n") + infolog);
		delete[] infolog;
		return false;
	}
	return true;
}

bool GLEShader::reload()
{
	if (_state == GLE_SHADER_CREATED_NO_FILES) {
		return false;
	}

	return init();
}

