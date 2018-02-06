#ifndef VERTEX_H
#define VERTEX_H

#include <glm.hpp>

namespace Core {
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vertex &other);
		Vertex(const glm::vec3 &position, const glm::vec4 &color);
		~Vertex() = default;

		glm::vec3 position;
		glm::vec4 color;
	};
};

#endif // !VERTEX_H
