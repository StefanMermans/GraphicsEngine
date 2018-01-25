#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#include "ConsoleProvider.h"

namespace Core {

	class Logger {
	public:
		static bool init();
		static bool log(const std::string &message);
		static bool logLine(const std::string &message);
	private:
		static ConsoleProvider * _consoleProviderPtr;
	};

}

#endif // !LOGGER_H
