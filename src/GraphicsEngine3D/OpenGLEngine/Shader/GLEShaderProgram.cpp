#include "GLEShaderProgram.h"
#include "../../Core/Logger.h"
#include "../../Core/BasicAssetLoader.h"

#include <fstream>
#include <GL\glew.h>
#include <gtc/type_ptr.hpp>


using Core::Logger;

GLEShaderProgram::GLEShaderProgram(const std::string &vsFilePath, const std::string &fsFilePath) :
	_vsFilePath(vsFilePath),
	_fsFilePath(fsFilePath)
{
	// Initialise the shader program
	_programId = glCreateProgram();
	_state = CREATED;
}

GLEShaderProgram::GLEShaderProgram()
{
	_programId = glCreateProgram();
	_state = CREATED_NO_FILES;
}

GLEShaderProgram::~GLEShaderProgram()
{
	Logger::logLine("~GLEShaderProgram()");
	
	// Unset the program and delete it.
	glUseProgram(0);
	glDeleteProgram(_programId);
}

bool GLEShaderProgram::init(std::string &vertShaderData, std::string &fragShaderData) {
	_programId = glCreateProgram();

	bool success = false;
	// Compile and attach the vertex shader
	GLuint vertexShaderId = 0;
	success = compile(vertShaderData.c_str(), _programId, GL_VERTEX_SHADER, vertexShaderId);
	if (!success) {
		return false;
	}
	glAttachShader(_programId, vertexShaderId);

	// Compile and attach the fragment shader
	GLuint fragShaderId = 0;
	success = success && compile(fragShaderData.c_str(), _programId, GL_FRAGMENT_SHADER, fragShaderId);
	if (!success) {
		return false;
	} else {
		_state = READY;
	}

	glAttachShader(_programId, fragShaderId);

	// Link the program
	glLinkProgram(_programId);

	// get the uniform from the modelViewMatrix
	_modelViewUniform = glGetUniformLocation(_programId, "modelViewProjectionMatrix");

	// We use vertex attribute 0 and 1. whatever that means.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	return success;
}

bool GLEShaderProgram::init() {
	// Read the shader code from the files
	std::string vertexShaderData =
		Core::BasicAssetLoader::readFile(_vsFilePath);
	std::string fragShaderData =
		Core::BasicAssetLoader::readFile(_fsFilePath);

	return init(vertexShaderData, fragShaderData);
}

bool GLEShaderProgram::compile(const char * shaderData, const GLuint & programId, const GLuint &shaderType, GLuint &shaderId) {
	// Create a shader
	shaderId = glCreateShader(shaderType);
	// Read the shader from the shader data.
	glShaderSource(shaderId, 1, &shaderData, NULL);
	// Compile the shader
	glCompileShader(shaderId);
	// Check for errors and return.
	return checkErrors(shaderId);
}

void GLEShaderProgram::use() {
	// Zet dit als actieve programma
	glUseProgram(_programId);
}

void GLEShaderProgram::setUniform(glm::mat4 viewMatrix) {
	glUniformMatrix4fv(_modelViewUniform, 1, 0, glm::value_ptr(viewMatrix));
}

bool GLEShaderProgram::checkErrors(GLuint shaderId) {
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
