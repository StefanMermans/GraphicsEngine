#ifndef GLE_SHADER_H
#define GLE_SHADER_H
#include <string>
#include <GL\glew.h>
#include <glm.hpp>
#include <iostream>
#include "GLEFragmentShader.h"
#include "GLEVertexShader.h"

class GLEShaderProgram
{
public:
	GLEShaderProgram(
		const std::string &vsFilePath,
		const std::string &fsFilePath);
	GLEShaderProgram();
	~GLEShaderProgram();

	bool init();
	
	void use();
	void setUniform(glm::mat4 viewMatrix);

	inline GLEShaderState state() const { return _state; };
private:
	GLEVertexShader _vertShader;
	GLEFragmentShader _fragShader;

	GLuint _programId;
	GLuint _modelViewUniform;
	GLuint _multiplier;

	GLEShaderState _state;
};

#endif // !GLE_SHADER_H
