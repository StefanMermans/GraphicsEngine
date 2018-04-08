#include "GLEController.h"
#include "GLEstdafx.h"
#include "../Core/Logger.h"
#include "Controller/FileListener.h"
#include "../Core/ObjLoader.h"
#include <string>
#include <thread>
#include "Controller\InputListener.h"
#include "../Core/ImageLoader.h"

using Core::Logger;

static bool lazy = false;
static bool resetPointer = true;

static GLEShaderProgram * shader;
static GLuint fboId;
static GLuint fboTextureId;
static GLuint rboId;

static GLEShaderProgram * outlineColorShader;

namespace ShaderSwitcher {
	static std::vector<GLEModel*> switchObjects;
	static std::vector<GLEShaderProgram*> shaders;
	static size_t currentShader = 0;

	GLEShaderProgram* nextShader() {
		currentShader++;
		if (currentShader >= shaders.size()) {
			currentShader = 0;
		}

		for (size_t i = 0; i < switchObjects.size(); i++) {
			switchObjects[i]->replaceShader(shaders[currentShader], false);
		}
		
		Core::Logger::logLine("Shader: " + std::to_string(currentShader));

		return shaders[currentShader];
	}

	GLEShaderProgram* previousShader() {
		if (currentShader == 0) {
			currentShader = shaders.size() - 1;
		}
		else {
			currentShader--;
		}

		for (size_t i = 0; i < switchObjects.size(); i++) {
			switchObjects[i]->replaceShader(shaders[currentShader], false);
		}
		
		Core::Logger::logLine("Shader: " + std::to_string(currentShader));

		return shaders[currentShader];
	}
}

namespace PostShaderSwitcher {
	static std::vector<GLEShaderProgram*> postShaders;
	static size_t currentPostShader = 0;

	void loadShader(const std::string& shaderPath) {
		GLEShaderProgram * postShader = new GLEShaderProgram{ shaderPath + ".vs", shaderPath + ".fs"};
		postShader->init();
		postShader->use();
		postShader->setInt("texInput0", 0);
		PostShaderSwitcher::postShaders.push_back(postShader);
	}

	GLEShaderProgram* nextShader() {
		currentPostShader++;
		if (currentPostShader >= postShaders.size()) {
			currentPostShader = 0;
		}

		Core::Logger::logLine("PostShader: " + std::to_string(currentPostShader));

		return postShaders[currentPostShader];
	}

	GLEShaderProgram* previousShader() {
		if (currentPostShader == 0) {
			currentPostShader = postShaders.size() - 1;
		}
		else {
			currentPostShader--;
		}

		Core::Logger::logLine("PostShader: " + std::to_string(currentPostShader));

		return postShaders[currentPostShader];
	}
}

GLEController::GLEController()
{
}

void GLEController::reloadShaders() {
	for (size_t i = 0; i < ShaderSwitcher::shaders.size(); i++) {
		ShaderSwitcher::shaders[i]->reload();
	}

	shader->reload();

	for (size_t i = 0; i < PostShaderSwitcher::postShaders.size(); i++) {
		PostShaderSwitcher::postShaders[i]->reload();
	}
}

void GLEController::init() {
	outlineColorShader = new GLEShaderProgram{ "../Assets/shaders/GLSL/outlineColor" };
	outlineColorShader->init();

	// Switchable shaders.
	GLEShaderProgram * newShader = new GLEShaderProgram{ "../Assets/shaders/GLSL/basic.vs", "../Assets/shaders/GLSL/basic.fs" };
	newShader->init();
	GLEShaderProgram * toonShader = new GLEShaderProgram{ "../Assets/shaders/GLSL/toonLighting"};
	toonShader->init();
	GLEShaderProgram * phongShader = new GLEShaderProgram{ "../Assets/shaders/GLSL/phong.vs", "../Assets/shaders/GLSL/phong.fs" };
	phongShader->init();
	GLEShaderProgram * waveShader = new GLEShaderProgram{ "../Assets/shaders/GLSL/wave.vs", "../Assets/shaders/GLSL/wave.fs" };
	waveShader->init();
	GLEShaderProgram * outlineShader = new GLEShaderProgram{ "../Assets/shaders/GLSL/outline.vs", "../Assets/shaders/GLSL/outline.fs" };
	outlineShader->init();

	ShaderSwitcher::shaders.push_back(toonShader);
	ShaderSwitcher::shaders.push_back(newShader);
	ShaderSwitcher::shaders.push_back(phongShader);
	ShaderSwitcher::shaders.push_back(waveShader);
	ShaderSwitcher::shaders.push_back(outlineShader);

	// Cube
	GLEModel * cubeModel = new GLEModel{ CUBE_VERTS, ShaderSwitcher::shaders[ShaderSwitcher::currentShader] };
	cubeModel->_transformation = {
		{ -1, -1.5, 0 },
		{  0,   0,  0 }
	};
	ShaderSwitcher::switchObjects.push_back(cubeModel);

	// Spehere
	Mesh sphereMesh = Core::ObjLoader::loadObj("../Assets/objects/sphere.obj");
	GLEModel * sphereModel = new GLEModel{ sphereMesh , ShaderSwitcher::shaders[ShaderSwitcher::currentShader] };
	sphereModel->_transformation = {
		{ -3, -1.5, 0},
		{ 0, 0, 0}
	};
	ShaderSwitcher::switchObjects.push_back(sphereModel);

	// The environment shader and object.
	shader = new GLEShaderProgram{ "../Assets/shaders/GLSL/roomLighting.vs", "../Assets/shaders/GLSL/roomLighting.fs" };
	shader->init();

	Mesh mesh = Core::ObjLoader::loadObj("../Assets/objects/room/room.obj");

	GLEModel * room = new GLEModel{ mesh, shader };
	//GLEModel * room = new GLEModel{ mesh, toonShader };
	room->_transformation = {
		{ -1, 0, 0 },
		{ 0, 0, 0 }
	};

	int width;
	int height;
	unsigned char* image = Core::ImageLoader::readImage("../Assets/objects/room/roomUv.png", &width, &height);
	room->addTexture(image, width, height);
	Core::ImageLoader::release(image);

	int width1;
	int height1;
	unsigned char* mikasa = Core::ImageLoader::readImage("../Assets/objects/room/mikasaUvAlpha.png", &width1, &height1);
	room->addTexture(mikasa, width1, height1);
	Core::ImageLoader::release(mikasa);

	this->models.push_back(room);

	std::thread * listenThread = new std::thread{[=]{
		FileListener listener{ L"../Assets/shaders/GLSL/" };
		while (true) {
			if (listener.listen()) {
				lazy = true;
				Logger::logLine("file changed!");
			}
		}
	} };

	// Post processing shader
	// Basic post shader
	PostShaderSwitcher::loadShader("../Assets/shaders/GLSL/post/basic");
	PostShaderSwitcher::loadShader("../Assets/shaders/GLSL/post/blur");
	PostShaderSwitcher::loadShader("../Assets/shaders/GLSL/post/colorWarp");
	PostShaderSwitcher::loadShader("../Assets/shaders/GLSL/post/greyScale");
	PostShaderSwitcher::loadShader("../Assets/shaders/GLSL/post/pixelate");
	PostShaderSwitcher::loadShader("../Assets/shaders/GLSL/post/wave");

	// Framebuffer object
	glGenTextures(1, &fboTextureId);
	glBindTexture(GL_TEXTURE_2D, fboTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTextureId, 0);

	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 2048, 2048);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// !FrameBuffer object
}

void GLEController::update(float deltaTime, Input * input)
{
	if (resetPointer) {
		glutSetCursor(GLUT_CURSOR_NONE);

		glutWarpPointer(1280 / 2, 720 / 2);

		_camera.update(deltaTime, input);
	}
	else {
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}

	for (size_t i = 0; i < ShaderSwitcher::switchObjects.size(); i++) {
		ShaderSwitcher::switchObjects[i]->_transformation._rotation.y += 1.0f * deltaTime;
		ShaderSwitcher::switchObjects[i]->_transformation._rotation.x += 1.0f * deltaTime;
		//ShaderSwitcher::centerObject->_transformation._rotation.z += 1.0f * deltaTime;
	}
	
	if (lazy) {
		lazy = false;
		reloadShaders();
	}

	for (size_t i = 0; i < models.size(); i++) {
		models[i]->update(deltaTime * 1.1f);
	}

	if (input->_keys[' '].isPressed()) {
		resetPointer = !resetPointer;
	}

	if (input->_keys[']'].isPressed()) {
		ShaderSwitcher::nextShader();
	}
	else if (input->_keys['['].isPressed()) {
		ShaderSwitcher::previousShader();
	}

	if (input->_keys['.'].isPressed() || input->_keys['>'].isPressed()) {
		PostShaderSwitcher::nextShader();
	}
	else if (input->_keys[','].isPressed() || input->_keys['<'].isPressed()) {
		PostShaderSwitcher::previousShader();
	}
}

void GLEController::render(glm::mat4 projection)
{
	// Framebuffer
	if (ShaderSwitcher::currentShader == ShaderSwitcher::shaders.size() - 1) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1280, 720);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		glViewport(0, 0, 2048, 2048);
	}

	glStencilMask(0xFF); // Update the stencil buffer, otherwise it won't be cleared!
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Render the objects
	glm::mat4 camLookat = _camera.calcLookAt();

	// Render the shaderswitcher objects using special methods.
	
	if (ShaderSwitcher::currentShader == ShaderSwitcher::shaders.size() -1) {
		glStencilMask(0x00); // Don't update the stencil buffer

		// Render basic objects
		for (size_t i = 0; i < models.size(); i++) {
			models[i]->render(projection, camLookat, _camera.pos);
		}

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0xFF); // Update the stencil buffer

		// Render the objects normally
		for (size_t i = 0; i < ShaderSwitcher::switchObjects.size(); i++) {
			ShaderSwitcher::switchObjects[i]->render(projection, camLookat, _camera.pos);
		}

		// Draw the oulint objects
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00); // Don't update the stencil buffer
		//glDisable(GL_DEPTH_TEST);

		for (size_t i = 0; i < ShaderSwitcher::switchObjects.size(); i++) {
			glm::mat4 model = glm::translate({}, ShaderSwitcher::switchObjects[i]->_transformation.position);
			model = glm::rotate(model, ShaderSwitcher::switchObjects[i]->_transformation._rotation.x, glm::vec3(1, 0, 0));
			model = glm::rotate(model, ShaderSwitcher::switchObjects[i]->_transformation._rotation.y, glm::vec3(0, 1, 0));
			model = glm::rotate(model, ShaderSwitcher::switchObjects[i]->_transformation._rotation.z, glm::vec3(0, 0, 1));
			model = glm::scale(model, { 1.2f, 1.2f, 1.2f });

			outlineColorShader->use();
			outlineColorShader->setVec3("cameraPos", _camera.pos);
			outlineColorShader->setMat4("modelViewProjectionMatrix", projection * (camLookat * model));
			outlineColorShader->setMat4("view", projection);
			outlineColorShader->setMat4("projection", camLookat);
			outlineColorShader->setMat4("model", model);
			outlineColorShader->setInt("time", glutGet(GLUT_ELAPSED_TIME));

			ShaderSwitcher::switchObjects[i]->render(projection, camLookat, _camera.pos, false);
		}
		
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glStencilMask(0x00);

		for (size_t i = 0; i < ShaderSwitcher::switchObjects.size(); i++) {
			ShaderSwitcher::switchObjects[i]->render(projection, camLookat, _camera.pos);
		}

		// Render basic objects
		for (size_t i = 0; i < models.size(); i++) {
			models[i]->render(projection, camLookat, _camera.pos);
		}

		// Framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		std::vector<glm::vec2> verts;
		verts.push_back({ -1, -1 });
		verts.push_back({ 1, -1 });
		verts.push_back({ 1, 1 });
		verts.push_back({ -1, 1 });

		GLEShaderProgram* postShader = PostShaderSwitcher::postShaders[PostShaderSwitcher::currentPostShader];
		postShader->use();
		float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
		postShader->setFloat("time", time);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboTextureId);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * 4, &verts[0]);
		glDrawArrays(GL_QUADS, 0, verts.size());
	}
}
