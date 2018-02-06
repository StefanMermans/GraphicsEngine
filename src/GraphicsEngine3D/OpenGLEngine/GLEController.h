#ifndef GLE_CONTROLLER_H
#define GLE_CONTROLLER_H

#include <vector>
#include "GLEModel.h"
#include "../Core/Vertex.h"
#include "GLEKey.h"
#include "../OpenGLEngine/Controller/GLEMouse.h"

#define GLE_MOUSE_BUTTON_NONE -1
#define GLE_KEY_COUNT sizeof(unsigned char) * 255

class GLEController {
public:
	GLEController();
	~GLEController() = default;

	void init();
	void update(float deltaTime);
	void keyDown(unsigned char key, int x, int y);
	void keyUp(unsigned char key, int x, int y);
	void render(glm::mat4 view);
	void mouseAction(
		int x,
		int y,
		int button = GLE_MOUSE_BUTTON_NONE,
		int state = GLE_MOUSE_BUTTON_NONE);
	void reloadShaders();

	std::vector<GLEModel*> models;
private:
	GLEKey _keys[GLE_KEY_COUNT];
	GLEMouse _mouse;

	const std::vector<Core::Vertex> _triangleVertices = {
		{ { -1, -1,  0 },{ 1, 0, 0, 1 } },
		{ { 1, -1,  0 },{ 0, 1, 0, 1 } },
		{ { -1,  1,  0 },{ 0, 0, 1, 1 } }
	};
	const std::vector<Core::Vertex> _cubeVertices = {
		// FRONT
		{ { -1,-1,-1 },{ 1, 0, 0, 1 } },
		{ { 1,-1,-1 },{ 1, 0, 0, 1 } },
		{ { -1, 1,-1 },{ 1, 0, 0, 1 } },

		{ { 1, 1,-1 },{ 1, 0, 0, 1 } },
		{ { 1,-1,-1 },{ 1, 0, 0, 1 } },
		{ { -1, 1,-1 },{ 1, 0, 0, 1 } },
		// BACK
		{ { -1,-1, 1 },{ 0, 0, 1, 1 } },
		{ { 1,-1, 1 },{ 0, 0, 1, 1 } },
		{ { -1, 1, 1 },{ 0, 0, 1, 1 } },

		{ { 1, 1, 1 },{ 0, 0, 1, 1 } },
		{ { 1,-1, 1 },{ 0, 0, 1, 1 } },
		{ { -1, 1, 1 },{ 0, 0, 1, 1 } },
		//
		{ { -1, 1,-1 },{ 0, 1, 0, 1 } },
		{ { 1, 1,-1 },{ 0, 1, 0, 1 } },
		{ { -1, 1, 1 },{ 0, 1, 0, 1 } },

		{ { 1, 1, 1 },{ 0, 1, 0, 1 } },
		{ { 1, 1,-1 },{ 0, 1, 0, 1 } },
		{ { -1, 1, 1 },{ 0, 1, 0, 1 } },
		//
		{ { -1,-1,-1 },{ 0, 0, 0, 1 } },
		{ { 1,-1,-1 },{ 0, 0, 0, 1 } },
		{ { -1,-1, 1 },{ 0, 0, 0, 1 } },

		{ { 1,-1, 1 },{ 0, 0, 0, 1 } },
		{ { 1,-1,-1 },{ 0, 0, 0, 1 } },
		{ { -1,-1, 1 },{ 0, 0, 0, 1 } },
		//
		{ { 1,-1,-1 },{ 1, 1, 1, 1 } },
		{ { 1, 1,-1 },{ 1, 1, 1, 1 } },
		{ { 1,-1, 1 },{ 1, 1, 1, 1 } },

		{ { 1, 1, 1 },{ 1, 1, 1, 1 } },
		{ { 1, 1,-1 },{ 1, 1, 1, 1 } },
		{ { 1,-1, 1 },{ 1, 1, 1, 1 } },
		//
		{ { -1,-1,-1 },{ 1, 0, 1, 1 } },
		{ { -1, 1,-1 },{ 1, 0, 1, 1 } },
		{ { -1,-1, 1 },{ 1, 0, 1, 1 } },

		{ { -1, 1, 1 },{ 1, 0, 1, 1 } },
		{ { -1, 1,-1 },{ 1, 0, 1, 1 } },
		{ { -1,-1, 1 },{ 1, 0, 1, 1 } }
	};
};

#endif // !GLE_CONTROLLER_H
