#ifndef GLE_CAMERA_H
#define GLE_CAMERA_H

#include "../GLEstdafx.h"

class GLECamera {
public:
	GLECamera();
	GLECamera(const glm::vec3 &position);
	~GLECamera() = default;

	glm::mat4 calcLookAt();

	glm::vec3 pos;
	glm::vec3 rotation;
};

#endif // !GLE_CAMERA_H