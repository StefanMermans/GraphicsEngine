#ifndef GLE_VERTEX_SHADER_H
#define GLE_VERTEX_SHADER_H

#include "../GLEstdafx.h"
#include <string>
#include "GLEShader.h"

class GLEVertexShader : public GLEShader {
public:
	GLEVertexShader(const std::string &filePath);
	GLEVertexShader();
	virtual ~GLEVertexShader() = default;

	bool init() override;
	bool attach(const GLuint &programId) override;
};

#endif // !GLE_SHADER_H
