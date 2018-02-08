#include "Vertex.h"
#include <glm.hpp>

using namespace Core;

Vertex::Vertex(const Vertex & other)
{
	position = glm::vec3(other.position);
	color = glm::vec4(other.color);
	texCoord = glm::vec2(other.texCoord);
	normal = glm::vec3(other.normal);
}

Vertex::Vertex(const glm::vec3 &position, const glm::vec4 &color) :
	position(position),
	color(color)
{
}

Core::Vertex::Vertex(const glm::vec3 &position, const glm::vec4 &color, const glm::vec2 &texCoord, const glm::vec3 &normal)
{
	this->position = position;
	this->color = color;
	this->texCoord = texCoord;
	this->normal = normal;
}
