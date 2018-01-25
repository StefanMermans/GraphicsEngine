#ifndef COUT_CONSOLE_PROVIDER_H
#define COUT_CONSOLE_PROVIDER_H

#include "ConsoleProvider.h"
#include <iostream>


namespace Core {

	class CoutConsoleProvider : public ConsoleProvider
	{
	public:
		CoutConsoleProvider();
		~CoutConsoleProvider() = default;

		bool log(const std::string &message) override;
		bool logLine(const std::string &message) override;
	};
}

#endif // !COUT_CONSOLE_PROVIDER_H

