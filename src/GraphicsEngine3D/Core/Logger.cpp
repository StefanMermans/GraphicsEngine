#include "Logger.h"

#ifdef _WIN32
#include "WinConsoleProvider.h"
#endif
#include "CoutConsoleProvider.h"


using namespace Core;

ConsoleProvider * Logger::_consoleProviderPtr = NULL;
bool Logger::_showLevel = true;

bool Logger::init(bool showLevel, bool forceCout)
{
	_showLevel = showLevel;
	if (forceCout) {
		_consoleProviderPtr = new CoutConsoleProvider();
	}
	else {
#ifdef _WIN32
		_consoleProviderPtr = new WinConsoleProvider();
#else // ENGINE_USE_DX
		_consoleProviderPtr = new CoutConsoleProvider();
#endif // ENGINE_USE_DX
	}

	return _consoleProviderPtr->getState() == CONSOLE_PROVIDER_INITIALISED;
}

bool Core::Logger::error(const std::string & message)
{
	if (_consoleProviderPtr == NULL) {
		return false;
	}

	if (_showLevel) {
		return _consoleProviderPtr->log(message, LogLevels::ERR);
	}
	return _consoleProviderPtr->log(message);
}

bool Core::Logger::errorLine(const std::string & message)
{
	if (_consoleProviderPtr == NULL) {
		return false;
	}

	if (_showLevel) {
		return _consoleProviderPtr->logLine(message, LogLevels::ERR);
	}
	return _consoleProviderPtr->logLine(message);
}

bool Core::Logger::warn(const std::string & message)
{
	if (_consoleProviderPtr == NULL) {
		return false;
	}

	if (_showLevel) {
		return _consoleProviderPtr->log(message, LogLevels::WARN);
	}
	return _consoleProviderPtr->log(message);
}

bool Core::Logger::warnLine(const std::string & message)
{
	if (_consoleProviderPtr == NULL) {
		return false;
	}

	if (_showLevel) {
		return _consoleProviderPtr->logLine(message, LogLevels::WARN);
	}
	return _consoleProviderPtr->logLine(message);
}

bool Logger::log(const std::string & message) {
	if (_consoleProviderPtr == NULL) {
		return false;
	}

	if (_showLevel) {
		return _consoleProviderPtr->log(message, LogLevels::INFO);
	}
	return _consoleProviderPtr->log(message);
}

bool Logger::logLine(const std::string & message)
{
	if (_consoleProviderPtr == NULL) {
		return false;
	}

	if (_showLevel) {
		return _consoleProviderPtr->logLine(message, LogLevels::INFO);
	}
	return _consoleProviderPtr->logLine(message);
}
