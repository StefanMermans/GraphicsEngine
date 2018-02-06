#ifndef GLE_MOUSE_H
#define GLE_MOUSE_H

#include "../GLEstdafx.h"
#include <vector>

#define GLE_DEFAULT_MOUSE_BUTTONS 5

struct GLEMouseButton {
	bool held;
	bool pressed;
};

class GLEMouse {
public:
	GLEMouse(const size_t &buttonCount = GLE_DEFAULT_MOUSE_BUTTONS);
	~GLEMouse() = default;

	void setPos(const glm::ivec2 &pos);
	void setState(const size_t &index, const bool &state);

	const glm::ivec2 getPos() const;
	const glm::ivec2 delta() const;
	bool isHeld(const size_t &index);
	bool isPressed(const size_t &index);
private:
	void growVector(const size_t &size);

	glm::ivec2 _delta;
	glm::ivec2 _lastPos;
	// We can't use a bool vector, because
	// they suck efficiency wise.
	std::vector<GLEMouseButton> _keys;

	bool _initial;
};

#endif // !GLE_MOUSE_H
