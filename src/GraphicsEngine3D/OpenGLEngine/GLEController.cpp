#include "GLEController.h"
#include "GLEstdafx.h"
#include "../Core/Logger.h"
#include "Controller/FileListener.h"
#include <string>
#include <thread>

using Core::Logger;

static bool lazy = false;

GLEController::GLEController()
{
	for (unsigned char i = 0; i < GLE_KEY_COUNT; i++) {
		_keys[i] = GLEKey{i};
	}
}

void GLEController::reloadShaders() {
	GLEShaderProgram * shaderBlue = new GLEShaderProgram{ "../Assets/shaders/GLSL/simple.vs", "../Assets/shaders/GLSL/distort1.fs" };
	shaderBlue->init();

	this->models[0]->replaceShader(shaderBlue);

	GLEShaderProgram * shaderRed = new GLEShaderProgram{ "../Assets/shaders/GLSL/simple.vs", "../Assets/shaders/GLSL/distort2.fs" };
	shaderRed->init();

	this->models[1]->replaceShader(shaderRed);
}

void GLEController::init()
{
	GLEShaderProgram * shaderBlue = new GLEShaderProgram{ "../Assets/shaders/GLSL/simple.vs", "../Assets/shaders/GLSL/distort1.fs" };
	shaderBlue->init();
	GLEModel * modelLeft = new GLEModel{ _cubeVertices, shaderBlue };
	modelLeft->_transformation = {
		{ 2, 0, -1 },
		{ 0, 0, 0 }
	};

	GLEShaderProgram * shaderRed = new GLEShaderProgram{ "../Assets/shaders/GLSL/simple.vs", "../Assets/shaders/GLSL/distort2.fs" };
	shaderRed->init();
	GLEModel * modelRight = new GLEModel{ _cubeVertices, shaderRed };
	modelRight->_transformation = {
		{ -2, 0, -1 },
		{ 0, 0, 0 }
	};

	this->models.push_back(modelLeft);
	this->models.push_back(modelRight);

	std::thread * listenThread = new std::thread{[=]{
		FileListener listener{ L"../Assets/shaders/GLSL/" };
		while (true) {
			if (listener.listen()) {
				lazy = true;
				Logger::logLine("file changed!");
			}
		}
	} };
	
}

void GLEController::update(float deltaTime)
{
	if (_keys['a'].isHeld()) {
		Logger::logLine("DT: " + std::to_string(deltaTime));
		_camera.pos.x -= 5 * deltaTime;
	}
	if (_keys['d'].isHeld()) {
		_camera.pos.x += 5 * deltaTime;
	}

	for (size_t i = 0; i < models.size(); i++) {
		models[i]->update(deltaTime / 1000.0f);
	}

	if (_keys['s'].isPressed() || lazy) {
		Logger::logLine("reloading shaders...");
		lazy = false;
		reloadShaders();
		Logger::logLine("Done!");
	}
}

void GLEController::keyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case VK_ESCAPE:
		glutLeaveMainLoop();
		break;
	default:
		_keys[key].down();
		break;
	}
}

void GLEController::keyUp(unsigned char key, int x, int y)
{
	_keys[key].up();
}

void GLEController::render(glm::mat4 view)
{
	//vermenigvuldig met een lookat
	//view *= glm::lookAt(glm::vec3(2, 0, 2), glm::vec3(2, 0, 0), glm::vec3(0, 1, 0));
	view *= _camera.calcLookAt();

	for (size_t i = 0; i < models.size(); i++) {
		models[i]->render(view);
	}
}

void GLEController::mouseAction(int x, int y, int button, int state)
{
	_mouse.setPos({ x, y });
	if (button != GLE_MOUSE_BUTTON_NONE) {
		_mouse.setState(button, state == GLUT_DOWN);
	}
}
