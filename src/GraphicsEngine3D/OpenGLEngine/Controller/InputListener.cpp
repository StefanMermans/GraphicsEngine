#include "InputListener.h"
#include <vector>

Input::Input()
{
	for (unsigned char i = 0; i < GLE_KEY_COUNT; i++) {
		_keys[i] = GLEKey{ i };
	}
}

void Input::keyDown(unsigned char key, int x, int y) {
	switch (key)
	{
	case VK_ESCAPE:
		glutLeaveMainLoop();
		break;
	default:
		_keys[tolower(key)].down();
		break;
	}
}
	
void Input::keyUp(unsigned char key, int x, int y) {
	_keys[tolower(key)].up();
}
	
void Input::mouseAction(int x, int y, int button, int state) {
	_mouse.setPos({ x, y });
	if (button != GLE_MOUSE_BUTTON_NONE) {
		_mouse.setState(button, state == GLUT_DOWN);
	}
}
