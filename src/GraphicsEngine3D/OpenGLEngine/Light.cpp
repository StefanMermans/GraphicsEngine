#include "Light.h"

#ifdef LIGHT

 Light::Light(const glm::vec3 &position, const glm::vec4 &color) : 
	GLEModel()
	position(position),
	color(color)
{

}

void Light::render(const glm::mat4 & view, const glm::mat4 & projection)
{
}

void Light::update(const float & deltaTime)
{
}
#endif
