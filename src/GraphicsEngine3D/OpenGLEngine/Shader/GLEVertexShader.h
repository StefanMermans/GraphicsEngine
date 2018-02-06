#ifndef GLE_SHADER_H
#define GLE_SHADER_H

#include "../GLEstdafx.h"
#include <string>

enum GLEShaderState
{
	GLE_SHADER_CREATED,
	GLE_SHADER_COMPILED,
	GLE_SHADER_FAILED
};

class GLEVertexShader {
public:
	GLEVertexShader(const std::string &filePath);
	virtual ~GLEVertexShader() = 0;

	virtual bool init() = 0;

	const GLenum type;
private:
	std::string _filePath;
	std::string _source;

	GLEShaderState _state;

	GLuint _id;
};

#endif // !GLE_SHADER_H
