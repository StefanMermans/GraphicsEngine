#include "GLECamera.h"

GLECamera::GLECamera()
{
	pos = { 0, 0, 2 };
	rotation = { 2, 0, 0 };
}

GLECamera::GLECamera(const glm::vec3 & position) :
	pos(position),
	rotation(0)
{
}

glm::mat4 GLECamera::calcLookAt()
{
	glm::vec3 target = pos + glm::vec3{ 0, 0, -1 };

	return glm::lookAt(pos, target, { 0, 1, 0 });
}
