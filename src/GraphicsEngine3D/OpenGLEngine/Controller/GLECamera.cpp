#include "GLECamera.h"
#include "InputListener.h"

GLECamera::GLECamera()
{
	pos = { 0, 0, 2 };
	rotation = { 2, 0, 0 };
	_worldUp = { 0, 1, 0 };
	updateCameraVectors();
}

GLECamera::GLECamera(const glm::vec3 & position) :
	pos(position),
	rotation(0)
{
}

void GLECamera::update(float deltaTime, Input * input)
{
	keyUpdate(deltaTime, input);
	mouseUpdate(deltaTime, input);
}

glm::mat4 GLECamera::calcLookAt()
{
	//pos = glm::vec3(0, -1, 0);
	//_front = glm::vec3(-1, -1, 0);

	return glm::lookAt(pos, pos + _front, _up);
}

void GLECamera::updateCameraVectors()
{
	glm::vec3 localFront;
	localFront.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	localFront.y = sin(glm::radians(_pitch));
	localFront.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(localFront);

	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}

void GLECamera::mouseUpdate(float deltaTime, Input * input)
{
	_yaw += input->_mouse.delta().x;
	_pitch += input->_mouse.delta().y;

	updateCameraVectors();
}

void GLECamera::keyUpdate(float deltaTime, Input * input)
{
	glm::vec3 horizontalFront(_front);
	horizontalFront.y = 0;

	if (input->_keys['a'].isHeld()) {
		pos += _right * 5.0f * deltaTime;
	}
	if (input->_keys['d'].isHeld()) {
		pos -= _right * 5.0f * deltaTime;
	}
	if (input->_keys['w'].isHeld()) {
		pos += horizontalFront * 5.0f * deltaTime;
	}
	if (input->_keys['s'].isHeld()) {
		pos -= horizontalFront * 5.0f * deltaTime;
	}
	if (input->_keys['e'].isHeld()) {
		pos -= _up * 5.0f * deltaTime;
	}
	if (input->_keys['q'].isHeld()) {
		pos += _up * 5.0f * deltaTime;
	}
}
