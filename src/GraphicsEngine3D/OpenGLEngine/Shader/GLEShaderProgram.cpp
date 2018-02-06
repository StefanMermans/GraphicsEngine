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

	// Link the program
	glLinkProgram(_programId);
	
	// get the uniform from the modelViewMatrix
	_modelViewUniform = glGetUniformLocation(_programId, "modelViewProjectionMatrix");

	return success;
}

void GLEShaderProgram::use() {
	// Zet dit als actieve programma
	glUseProgram(_programId);
}

void GLEShaderProgram::setUniform(glm::mat4 viewMatrix) {
	glUniformMatrix4fv(_modelViewUniform, 1, 0, glm::value_ptr(viewMatrix));
}

