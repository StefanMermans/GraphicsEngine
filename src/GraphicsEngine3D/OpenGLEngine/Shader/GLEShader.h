#ifndef GLE_SHADER
#define GLE_SHADER

#include "../GLEstdafx.h"
#include <string>

enum GLEShaderState {
	GLE_SHADER_CREATED,
	GLE_SHADER_CREATED_NO_FILES,
	GLE_SHADER_COMPILATION_FAILED,
	GLE_SHADER_COMPILED
};

class GLEShader {
public: 
	GLEShader(const std::string &filePath, const GLenum &shaderType);
	GLEShader(const GLenum &shaderType);
	virtual ~GLEShader();

	virtual bool init() = 0;
	GLEShaderState getState() const;
	virtual bool attach(const GLuint &programId);
	static bool checkErrors(const GLuint &shaderId);
protected:
	bool compile(const char * shaderData);

	std::string _filePath;
	GLuint _id;
private:
	GLEShaderState _state;
};

#endif // !GLE_SHADER
