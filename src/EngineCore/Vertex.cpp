#include "Vertex.h"

Vertex::Vertex(const Vertex & other)
{
	position = other.position;
	color = other.color;
}

Vertex::Vertex(const Vec3f &position, const Vec4f &color) {
	this->position = position;
	this->color = color;
}
