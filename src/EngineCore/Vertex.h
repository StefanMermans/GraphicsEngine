#ifndef VERTEX_H
#define VERTEX_H

#include "Vec.h"

class Vertex
{
public:
	Vertex() = default;
	Vertex(const Vertex &other);
	Vertex(const Vec3f &position, const Vec4f &color);
	virtual ~Vertex() = default;

	Vec3f position;
	Vec4f color;
};

#endif // !VERTEX_H
