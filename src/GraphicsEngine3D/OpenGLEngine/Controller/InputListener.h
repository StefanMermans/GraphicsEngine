#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "../GLEstdafx.h"
#include "../GLEKey.h"
#include "GLEMouse.h"

#define GLE_MOUSE_BUTTON_NONE -1
#define GLE_KEY_COUNT sizeof(unsigned char) * 255

class Input{
public:
	Input();
	~Input() = default;

	void keyDown(unsigned char key, int x, int y);
	void keyUp(unsigned char key, int x, int y);
	void mouseAction(
		int x,
		int y,
		int button = GLE_MOUSE_BUTTON_NONE,
		int state = GLE_MOUSE_BUTTON_NONE);

	GLEMouse _mouse;
	GLEKey _keys[GLE_KEY_COUNT];
};

#endif // !INPUT_LISTENER_H
