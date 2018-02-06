#ifndef GLE_FRAGMENT_SHADER
#define GLE_FRAGMENT_SHADER

#include "GLEShader.h"
#include "../GLEstdafx.h"

class GLEFragmentShader : public GLEShader {
public:
	GLEFragmentShader(const std::string &filePath);
	GLEFragmentShader();
	~GLEFragmentShader() = default;

	bool init() override;
};

#endif // !GLE_FRAGMENT_SHADER
