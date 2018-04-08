#include "GLEKey.h"

GLEKey::GLEKey() {
	_held = false;
	_registered = false;
}

GLEKey::GLEKey(const GLEKey & other)
{
	_id = other.getId();
	_held = false;
	_registered = false;
}

GLEKey::GLEKey(const unsigned char &id) :
	_id(id)
{
	_held = false;
	_registered = false;
}

void GLEKey::down() {
	if (!_held) {
		_registered = false;
	}
	_held = true;	
}

void GLEKey::up() {
	_registered = false;
	_held = false;
}

bool GLEKey::isPressed() {
	if (_held && !_registered) {
		_registered = true;
		return true;
	}
	return false;
}

bool GLEKey::isHeld() const {
	return _held;
}

unsigned char GLEKey::getId() const {
	return _id;
}

void GLEKey::setId(const unsigned char &id) {
	_id = id;
}
