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
		Vertex(const glm::vec3 &position,
			const glm::vec4 &color,
			const glm::vec2 &texCoord,
			const glm::vec3 &normal);
		~Vertex() = default;

		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};
};

#endif // !VERTEX_H
