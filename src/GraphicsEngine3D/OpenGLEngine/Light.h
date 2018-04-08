#ifndef LIGHT_H
#define LIGHT_H

#include "GLEstdafx.h"
#include "../Core/Vertex.h"
#include "GLEModel.h"

#ifdef LIGHT
class Light : public GLEModel {
public:
	Light(const glm::vec3 &position, const glm::vec4 &color);
	~Light() = default;

	void render(const glm::mat4 &view, const glm::mat4 &projection) override;
	void update(const float &deltaTime) override;

	glm::vec3 position;
	glm::vec3 color;
private: 

};
#endif

#endif // !LIGHT_H
