#include "ConsoleProvider.h"

using namespace Core;

ConsoleProvider::ConsoleProvider()
{
	_state = CONSOLE_PROVIDER_INITIALISED;
}

unsigned int ConsoleProvider::getState() const
{
	return _state;
}
