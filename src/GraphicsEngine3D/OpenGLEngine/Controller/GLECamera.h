#ifndef GLE_CAMERA_H
#define GLE_CAMERA_H

#include "../GLEstdafx.h"
#include "InputListener.h"

class GLECamera {
public:
	GLECamera();
	GLECamera(const glm::vec3 &position);
	~GLECamera() = default;

	void update(float deltaTime, Input * input);

	glm::mat4 calcLookAt();

	glm::vec3 pos;
	glm::vec3 rotation;

	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;

	float _yaw;
	float _pitch;
private:
	void updateCameraVectors();
	void mouseUpdate(float deltaTime, Input* input);
	void keyUpdate(float deltaTime, Input* input);
};

#endif // !GLE_CAMERA_H