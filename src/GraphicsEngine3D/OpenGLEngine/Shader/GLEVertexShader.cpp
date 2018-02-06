#include "GLEVertexShader.h"

GLEVertexShader::GLEVertexShader(const std::string & filePath) :
	type(GL_VERTEX_SHADER),
	_filePath(filePath)
{
	_state = GLE_SHADER_CREATED;
}
