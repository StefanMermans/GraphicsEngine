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
	_transformation._rotation.z += rotate;
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

void GLEModel::render(const glm::mat4 &view, const glm::mat4 &projection) {
	glm::mat4 model = glm::translate({}, _transformation.position);
	model = glm::rotate(model, _transformation._rotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, _transformation._rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, _transformation._rotation.z, glm::vec3(0, 0, 1));

	// Pass parameters to the shader and use it
	_shaderProgram->use();
	_shaderProgram->setMat4("modelViewProjectionMatrix", view * (projection * model));
	_shaderProgram->setMat4("view", view);
	_shaderProgram->setMat4("projection", projection);
	_shaderProgram->setMat4("model", model);

	//for (int i = 0; i <= 12; i++) {
	//	float * data = (float*)_vertices.data() + i; 
	//	std::cout << i << ": " << *data << std::endl;
	//}

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (float*)_vertices.data());
	glEnableVertexAttribArray(0);
	// Colors
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (float*)_vertices.data() + 3);
	glEnableVertexAttribArray(1);
	// TexCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (float*)_vertices.data() + 7);
	glEnableVertexAttribArray(2);
	// Normals
	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertex), (float*)_vertices.data() + 9);
	glEnableVertexAttribArray(3);

	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}
