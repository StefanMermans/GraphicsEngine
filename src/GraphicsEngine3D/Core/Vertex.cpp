#include "Vertex.h"
#include <glm.hpp>

using namespace Core;

Vertex::Vertex(const Vertex & other)
{
	position = glm::vec3(other.position);
	color = glm::vec4(other.color);
}

Vertex::Vertex(const glm::vec3 &position, const glm::vec4 &color) :
	position(position),
	color(color)
{
}