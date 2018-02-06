#include "GLEModel.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../Core/Vertex.h"
#include "../Core/Logger.h"

using Core::Vertex;
using Core::Logger;

GLEModel::GLEModel(std::vector<Vertex> vertices, GLEShaderProgram * shader) :
	_vertices(vertices),
	_shaderProgram(shader)
{
	_transformation = {};
}

GLEModel::~GLEModel()
{
	Logger::logLine("~GLEModel()");

	delete _shaderProgram;
}

void GLEModel::update(float rotate) {
	_transformation._rotation.y += rotate;
	_transformation._rotation.x += rotate;
}

void GLEModel::replaceShader(GLEShaderProgram * shader)
{
	// Delete existing shader, if any
	if (_shaderProgram) {
		delete _shaderProgram;
	}
	// Copy the pointer to the new shader.
	_shaderProgram = shader;
}

void GLEModel::render(const glm::mat4 view) {
	glm::mat4 mvp = glm::translate(view, _transformation.position);
	mvp = glm::rotate(mvp, _transformation._rotation.y, glm::vec3(0, 1, 0));
	mvp = glm::rotate(mvp, _transformation._rotation.x, glm::vec3(1, 0, 0));

	// Pass parameters to the shader and use it
	_shaderProgram->use();
	_shaderProgram->setUniform(mvp);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), _vertices.data());
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (float*)(_vertices.data()) + 3);

	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}
