#include "GLEMouse.h"


GLEMouse::GLEMouse(const size_t &buttonCount) : 
	_initial(true)
{

	growVector(buttonCount);
}

void GLEMouse::setPos(const glm::ivec2 &pos)
{
	// In the initial state the mouse delta will be invalid
	if (_initial) {
		_initial = false;
		_lastPos = pos;
	}
	else {
		// Calculate mouse movement
		_delta = _lastPos - pos;
		// Set _lastPos to the newest value
		_lastPos = pos;
	}
}

bool GLEMouse::isHeld(const size_t &index)
{
	// Grow keys vector to fit demand
	growVector(index);

	// Make sure the held variable isn't going to be modified.
	return _keys[index].held == true;
}

bool GLEMouse::isPressed(const size_t & index)
{
	// Grow keys vector to fit demand
	growVector(index);

	if (_keys[index].pressed) {
		_keys[index].pressed = false;
		return true;
	}
	return false;
}

void GLEMouse::setState(const size_t & index, const bool & state)
{
	// Grow keys vector to fit demand
	growVector(index);

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

void GLEMouse::growVector(const size_t & size)
{
	while (size >= _keys.size()) {
		_keys.push_back({ false, false });
	}
}
