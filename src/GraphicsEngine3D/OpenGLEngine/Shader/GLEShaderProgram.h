#ifndef GLE_SHADER_H
#define GLE_SHADER_H
#include <string>
#include <GL\glew.h>
#include <glm.hpp>
#include <iostream>

enum GLEShaderState {
	CREATED,
	CREATED_NO_FILES,
	READY,
};

class GLEShaderProgram
{
public:
	GLEShaderProgram(
		const std::string &vsFilePath,
		const std::string &fsFilePath);
	GLEShaderProgram();
	~GLEShaderProgram();

	bool init();
	bool init(
		std::string &vertShaderData,
		std::string &fragShaderData);

	void use();
	void setUniform(glm::mat4 viewMatrix);

	static bool checkErrors(GLuint shaderId);
	inline GLEShaderState state() const { return _state; };
private:
	static bool compile(const char* shaderData, const GLuint &programId, const GLuint &shaderType, GLuint &shaderId);

	std::string _vsFilePath;
	std::string _fsFilePath;

	GLuint _programId;
	GLuint _modelViewUniform;

	GLEShaderState _state;
};

#endif // !GLE_SHADER_H
