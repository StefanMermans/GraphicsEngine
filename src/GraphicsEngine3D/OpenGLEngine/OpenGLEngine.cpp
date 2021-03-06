#include "GLEstdafx.h"
#include "OpenGLEngine.h"
#include <iostream>
#include <fstream>
#include <vector>

#include "OpenGLEngine.h"
#include "../Core/Vertex.h"
#include "GLEModel.h"
#include "../Core/Logger.h"
#include "Shader/GLEShaderProgram.h"
#include <thread>
#include "Controller/InputListener.h"

using Core::Logger;
using Core::Vertex;

static OpenGLEngine * enginePtr = NULL;
static GLEShaderProgram * postShader = NULL;

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
	glEnable(GL_STENCIL_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

	_inputListener = new Input();

	// Create callbacks to the OpenGL engine's member functions.
	glutMouseFunc([](int button, int state, int x, int y) {
		enginePtr->_inputListener->mouseAction(x, y, button, state);
	});
	glutMotionFunc([](int x, int y) {
		enginePtr->_inputListener->mouseAction(x, y);
	});
	glutPassiveMotionFunc([](int x, int y) {
		enginePtr->_inputListener->mouseAction(x, y);
	});
	glutDisplayFunc([]() {
		enginePtr->display();
	});
	glutReshapeFunc([](int newWidth, int newHeight) {
		enginePtr->reshape(newWidth, newHeight);
	});
	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		enginePtr->_inputListener->keyUp(key, x, y);
	});
	glutKeyboardFunc([](unsigned char key, int x, int y) {
		enginePtr->_inputListener->keyDown(key, x, y);
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
	//begin met een perspective matrix
	glm::mat4 view = glm::perspective(
		80.0f,
		_screenSize.x / (float)_screenSize.y,
		0.01f,
		100.0f
	);

	// Draw objects
	_controller.render(view);
	
	glutSwapBuffers();
}

void OpenGLEngine::reshape(int newWidth, int newHeight)
{
	_screenSize.x = newWidth;
	_screenSize.y = newHeight;
	glutPostRedisplay();
}

void OpenGLEngine::update()
{
	float time = (float)(glutGet(GLUT_ELAPSED_TIME));
	float deltaTime = (time - _lastTime) * 0.001f;
	_lastTime = time;

	_controller.update(deltaTime, _inputListener);

	glutPostRedisplay();
}
