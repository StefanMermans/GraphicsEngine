#include "GLEShaderProgram.h"
#include "../../Core/Logger.h"
#include "../../Core/BasicAssetLoader.h"

#include <fstream>
#include <GL\glew.h>
#include <gtc/type_ptr.hpp>


using Core::Logger;

GLEShaderProgram::GLEShaderProgram(const std::string &vsFilePath, const std::string &fsFilePath) :
	_vertShader(vsFilePath),
	_fragShader(fsFilePath)
{
	// Initialise the shader program
	_programId = glCreateProgram();
	_state = GLE_SHADER_CREATED;
}

GLEShaderProgram::GLEShaderProgram(const std::string & generalPath)  :
	_vertShader(generalPath + ".vs"),
	_fragShader(generalPath + ".fs")
{
	// Initialise the shader program
	_programId = glCreateProgram();
	_state = GLE_SHADER_CREATED;
}

GLEShaderProgram::GLEShaderProgram()
{
	_programId = glCreateProgram();
	_state = GLE_SHADER_CREATED_NO_FILES;
}

GLEShaderProgram::~GLEShaderProgram()
{
	Logger::logLine("~GLEShaderProgram()");
	
	// Unset the program and delete it.
	glUseProgram(0);
	glDeleteProgram(_programId);
}

bool GLEShaderProgram::init() {
	bool success = false;
	// Compile and attach the vertex shader
	success = _vertShader.init();
	if (!success) {
		return false;
	}
	_vertShader.attach(_programId);

	// Compile and attach the fragment shader
	success = success && _fragShader.init();
	if (!success) {
		return false;
	}
	_state = GLE_SHADER_COMPILED;
	_fragShader.attach(_programId);

	glBindAttribLocation(_programId, 0, "a_position");		// zet de positie op vertex attribuut 0
	glBindAttribLocation(_programId, 1, "a_color");			// zet de kleur op vertex attribuut 1
	glBindAttribLocation(_programId, 2, "a_texcoord");		// zet de texcoord op vertex attribuut 2
	glBindAttribLocation(_programId, 3, "a_normal");		// zet de texcoord op vertex attribuut 2

	// Link the program
	glLinkProgram(_programId);

	return success;
}

bool GLEShaderProgram::reload()
{
	if (_vertShader.reload() && _fragShader.reload()) {
		return init();
	}
	return false;
}

void GLEShaderProgram::use() {
	glUseProgram(_programId);
}

bool GLEShaderProgram::setVec3(const std::string & key, const glm::vec3 & value)
{
	GLuint location = glGetUniformLocation(_programId, key.c_str());
	glUniform3fv(location, 1, glm::value_ptr(value));

	return true;
}

bool GLEShaderProgram::setVec4(const std::string & key, const glm::vec4 & value)
{
	GLuint location = glGetUniformLocation(_programId, key.c_str());
	glUniform4fv(location, 1, glm::value_ptr(value));

	return true;
}

bool GLEShaderProgram::setMat3(const std::string & key, const glm::mat3 & value)
{
	GLuint location = glGetUniformLocation(_programId, key.c_str());
	glUniformMatrix3fv(location, 1, 0, glm::value_ptr(value));

	return true;
}

bool GLEShaderProgram::setMat4(const std::string & key, const glm::mat4 & value)
{
	GLuint location = glGetUniformLocation(_programId, key.c_str());
	glUniformMatrix4fv(location, 1, 0, glm::value_ptr(value));

	return true;
}

bool GLEShaderProgram::setFloat(const std::string & key, const GLfloat &value)
{
	GLuint location = glGetUniformLocation(_programId, key.c_str());
	glUniform1f(location, value);

	return true;
}

bool GLEShaderProgram::setInt(const std::string & key, const GLint & value)
{
	GLuint location = glGetUniformLocation(_programId, key.c_str());
	glUniform1i(location, value);

	return true;
}
