#include "GLEModel.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../Core/Vertex.h"
#include "../Core/Logger.h"

using Core::Vertex;
using Core::Logger;

GLEModel::GLEModel(const std::vector<Vertex> &vertices, GLEShaderProgram * shader) :
	_shaderProgram(shader)
{
	ObjGroup group;
	group.vertices = vertices;

	_mesh = Mesh{ {group} };
	_transformation = {};
}

GLEModel::GLEModel(const Mesh & mesh, GLEShaderProgram * shader) :
	_mesh(mesh),
	_shaderProgram(shader)
{
}

GLEModel::~GLEModel()
{
	Logger::logLine("~GLEModel()");

	delete _shaderProgram;
}

void GLEModel::update(const float &deltaTime) {
	//_transformation._rotation.y += deltaTime;
	//_transformation._rotation.x += deltaTime;
	//_transformation._rotation.z += deltaTime;
}

void GLEModel::replaceShader(GLEShaderProgram * shader, bool remove)
{
	// Delete existing shader, if any
	if (remove && _shaderProgram) {
		delete _shaderProgram;
	}
	// Copy the pointer to the new shader.
	_shaderProgram = shader;
}

const GLuint& GLEModel::addTexture(unsigned char * texture, const int& width, const int& height) {
	_shaderProgram->use();

	_textures.push_back(0);
	size_t textureIndex = _textures.size() - 1;

	glGenTextures(1, &_textures[textureIndex]);
	glBindTexture(GL_TEXTURE_2D, _textures[textureIndex]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	std::string textureString = "texInput" + std::to_string(textureIndex);
	_shaderProgram->setInt(textureString, textureIndex);

	return _textures[textureIndex];
}

void GLEModel::render(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &cameraPos, const bool& useShader) {
	// Pass parameters to the shader and use it
	if (useShader) {
		glm::mat4 model = glm::translate({}, _transformation.position);
		model = glm::rotate(model, _transformation._rotation.x, glm::vec3(1, 0, 0));
		model = glm::rotate(model, _transformation._rotation.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, _transformation._rotation.z, glm::vec3(0, 0, 1));

		_shaderProgram->use();
		_shaderProgram->setVec3("cameraPos", cameraPos);
		_shaderProgram->setMat4("modelViewProjectionMatrix", view * (projection * model));
		_shaderProgram->setMat4("view", view);
		_shaderProgram->setMat4("projection", projection);
		_shaderProgram->setMat4("model", model);
		_shaderProgram->setInt("time", glutGet(GLUT_ELAPSED_TIME));
	}

	if (_textures.size() != 0) {
		for (size_t i = 0; i < _textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, _textures[i]);
		}
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	_mesh.draw();
}
