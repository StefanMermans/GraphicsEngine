#ifndef LIGHT_H
#define LIGHT_H

#include "GLEstdafx.h"
#include "../Core/Vertex.h"

class Light {
public:
	Light(const glm::vec3 &position, const glm::vec4 &color);
	~Light() = default;

	glm::vec3 position;
	glm::vec3 color;
private: 

};

#endif // !LIGHT_H
