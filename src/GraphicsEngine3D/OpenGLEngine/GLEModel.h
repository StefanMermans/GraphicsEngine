#ifndef GLE_MODEL_H
#define GLE_MODEL_H
#include <vector>
#include <glm.hpp>

#include "Shader/GLEShaderProgram.h"
#include "../Core/Vertex.h"

struct Transformation {
	glm::vec3 position;
	glm::vec3 _rotation;
};

class GLEModel
{
public:
	GLEModel(std::vector<Core::Vertex> vertices, GLEShaderProgram * shader);
	~GLEModel();

	void render(const glm::mat4 &view, const glm::mat4 &projection);
	void update(float rotate);
	void replaceShader(GLEShaderProgram * shader);

	Transformation _transformation;
private:
	const std::vector<Core::Vertex> _vertices;
	GLEShaderProgram * _shaderProgram;
};

#endif // !GLE_MODEL_H
