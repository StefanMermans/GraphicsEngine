#include "GLEstdafx.h"
#include "OpenGLEngine.h"
#include <iostream>
#include <fstream>
#include <vector>

#include "OpenGLEngine.h"
#include "../Core/Vertex.h"
#include "GLEModel.h"
#include "../Core/Logger.h"
#include <thread>

using Core::Logger;
using Core::Vertex;

static OpenGLEngine * enginePtr = NULL;

#ifdef WIN32
void GLAPIENTRY onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
#else
void onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
{
	if (source == 33352) {
		Logger::errorLine(message);
	}
	else {
		//Logger::errorLine(message);
		//throw std::exception(message);
	}
}

void OpenGLEngine::init() {
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(0.4f, 0.5f, 0.7f, 1.0f);

	if (glDebugMessageCallback) {
		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glEnable(GL_DEBUG_OUTPUT);
	}

	_rotation = 0;
	_lastTime = (float)(glutGet(GLUT_ELAPSED_TIME));
}

OpenGLEngine::OpenGLEngine(int argc, char * argv[]) {
	enginePtr = this;

	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Modern OpenGL");

	// Create callbacks to the OpenGL engine's member functions.
	glutMouseFunc([](int button, int state, int x, int y) {
		enginePtr->mouseButton(button, state, x, y);
	});
	glutMotionFunc([](int x, int y) {
		enginePtr->activeMouse(x, y);
	});
	glutPassiveMotionFunc([](int x, int y) {
		enginePtr->passiveMouse(x, y);
	});
	glutDisplayFunc([]() {
		enginePtr->display();
	});
	glutReshapeFunc([](int newWidth, int newHeight) {
		enginePtr->reshape(newWidth, newHeight);
	});
	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		enginePtr->keyUp(key, x, y);
	});
	glutKeyboardFunc([](unsigned char key, int x, int y) {
		enginePtr->keyDown(key, x, y);
	});
	glutIdleFunc([]() {
		enginePtr->update();
	});
}

int OpenGLEngine::run() {
	init();
	_controller.init();

	glutMainLoop();

	return 0;
}

void OpenGLEngine::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//begin met een perspective matrix
	glm::mat4 view = glm::perspective(
		80.0f,
		_screenSize.x / (float)_screenSize.y,
		0.01f,
		100.0f
	);
	
	_controller.render(view);

	glutSwapBuffers();
}

void OpenGLEngine::reshape(int newWidth, int newHeight)
{
	_screenSize.x = newWidth;
	_screenSize.y = newHeight;
	glutPostRedisplay();
}

void OpenGLEngine::keyDown(unsigned char key, int x, int y)
{
	_controller.keyDown(key, x, y);
}

void OpenGLEngine::keyUp(unsigned char key, int x, int y)
{
	_controller.keyUp(key, x, y);
}

void OpenGLEngine::mouseButton(int button, int state, int x, int y)
{
	_controller.mouseAction(x, y, button, state);
}

void OpenGLEngine::passiveMouse(int x, int y)
{
	_controller.mouseAction(x, y);
}

void OpenGLEngine::activeMouse(int x, int y)
{
	_controller.mouseAction(x, y);
}

void OpenGLEngine::update()
{
	float time = (float)(glutGet(GLUT_ELAPSED_TIME));
	float deltaTime = (time - _lastTime) * 0.001f;
	_lastTime = time;

	_controller.update(deltaTime);

	glutPostRedisplay();
}
