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

	bool setVec3(const std::string &key, const glm::vec3 &value);
	bool setVec4(const std::string &key, const glm::vec4 &value);
	bool setMat3(const std::string &key, const glm::mat3 &value);
	bool setMat4(const std::string &key, const glm::mat4 &value);
	bool setFloat(const std::string &key, const GLfloat &value);

	inline GLEShaderState state() const { return _state; };
private:
	GLEVertexShader _vertShader;
	GLEFragmentShader _fragShader;

	GLuint _programId;

	GLEShaderState _state;
};

#endif // !GLE_SHADER_H
