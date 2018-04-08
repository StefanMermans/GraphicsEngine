#include "GLEMouse.h"


GLEMouse::GLEMouse(const size_t &buttonCount) : 
	_initial(true)
{
	allocVector(buttonCount);
}

void GLEMouse::setPos(const glm::ivec2 &pos)
{
#ifdef RELATIVE_MOUSE
	// In the initial state the mouse delta will be invalid
	if (_initial) {
		_initial = false;
		_lastPos = pos;
	}
	else {
		// Calculate mouse movement
		_delta.x = _lastPos.x - pos.x;
		_delta.y = pos.y - _lastPos.y;
		//_delta = _lastPos - pos;
		// Set _lastPos to the newest value
		_lastPos = pos;
	}
#else
	if (_initial) {
		_initial = false;
		_delta = glm::vec2();
	}
	else {
		glm::ivec2 center(1280/2, 720/2);

		_delta.y = pos.y - center.y;
		_delta.x = center.x - pos.x;
	}
#endif // RELATIVE_MOUSE

	
}

bool GLEMouse::isHeld(const size_t &index)
{
	// Grow keys vector to fit demand
	allocVector(index);

	// Make sure the held variable isn't going to be modified.
	return _keys[index].held == true;
}

bool GLEMouse::isPressed(const size_t & index)
{
	// Grow keys vector to fit demand
	allocVector(index);

	if (_keys[index].pressed) {
		_keys[index].pressed = false;
		return true;
	}
	return false;
}

void GLEMouse::setState(const size_t & index, const bool & state)
{
	// Grow keys vector to fit demand
	allocVector(index);

	if (!_keys[index].held && state) {
		_keys[index].pressed = true;
	}

	// Set the state of the keys
	_keys[index].held = state;
}

const glm::ivec2 GLEMouse::getPos() const
{
	return _lastPos;
}

const glm::ivec2 GLEMouse::delta() const
{
	return _delta;
}

void GLEMouse::allocVector(const size_t & size)
{
	while (size >= _keys.size()) {
		_keys.push_back({ false, false });
	}
}

