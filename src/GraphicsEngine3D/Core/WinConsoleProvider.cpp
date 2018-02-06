#include "WinConsoleProvider.h"

#ifdef _WIN32

using namespace Core;

WinConsoleProvider::WinConsoleProvider() : 
	ConsoleProvider()
{
	int console = AllocConsole();
	_stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (_stdHandle != INVALID_HANDLE_VALUE && console != 0) {
		_state = CONSOLE_PROVIDER_INITIALISED;
	}
	else {
		_state = CONSOLE_PROVIDER_FAILED;
		_stdHandle = NULL;
	}
}

bool WinConsoleProvider::log(const std::string & message)
{
	if (_state != CONSOLE_PROVIDER_INITIALISED) {
		return false;
	}

	const char * cStr = message.c_str();
	WriteConsole(_stdHandle, cStr, message.size(), NULL, NULL);

	return true;
}

bool WinConsoleProvider::logLine(const std::string & message)
{
	return log(message + "\n");
}

bool Core::WinConsoleProvider::log(const std::string & message, LogLevels level)
{
	return log(_levelNames[level] + message);
}

bool Core::WinConsoleProvider::logLine(const std::string & message, LogLevels level)
{
	return logLine(_levelNames[level] + message);
}

#endif // _WIN32
