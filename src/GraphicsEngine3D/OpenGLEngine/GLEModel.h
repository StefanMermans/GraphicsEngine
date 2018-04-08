#ifndef GLE_MODEL_H
#define GLE_MODEL_H
#include <vector>
#include <glm.hpp>

#include "Shader/GLEShaderProgram.h"
#include "../Core/Mesh/Mesh.h"
#include "../Core/Vertex.h"

struct Transformation {
	glm::vec3 position;
	glm::vec3 _rotation;
};

class GLEModel
{
public:
	GLEModel(const std::vector<Core::Vertex> &vertices, GLEShaderProgram * shader);
	GLEModel(const Mesh &mesh, GLEShaderProgram * shader);
	virtual ~GLEModel();

	virtual void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &cameraPos = {}, const bool &useShader = true);
	virtual void update(const float &deltaTime);
	void replaceShader(GLEShaderProgram * shader, bool remove=true);
	const GLuint& addTexture(unsigned char* texture, const int& width, const int& height);

	Transformation _transformation;
protected:
	Mesh _mesh;
	
	GLEShaderProgram * _shaderProgram;

	std::vector<GLuint> _textures;
};

#endif // !GLE_MODEL_H
