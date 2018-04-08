#ifndef OPENGL_ENGINE_H
#define OPENGL_ENGINE_H

#include "GLEModel.h"
#include <vector>
#include "GLEController.h"
#include "Controller\InputListener.h"

class OpenGLEngine {
public:
	OpenGLEngine(int argc, char * argv[]);
	~OpenGLEngine() = default;

	int run();
	void init();

	void display();
	void reshape(int newWidth, int newHeight);
	void keyDown(unsigned char key, int x, int y);
	void keyUp(unsigned char key, int x, int y);
	void mouseButton(int button, int state, int x, int y);
	void passiveMouse(int x, int y);
	void activeMouse(int x, int y);
	void update();
private:
	Input * _inputListener;
	glm::ivec2 _screenSize;
	std::vector<GLEModel> _models;
	float _rotation;
	float _lastTime;

	GLEController _controller;
};

#endif // OPENGL_ENGINE_H
