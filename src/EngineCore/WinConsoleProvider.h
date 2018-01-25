#ifndef WIN_CONSOLE_PROVIDER_H
#define WIN_CONSOLE_PROVIDER_H

#ifdef _WIN32

#include <Windows.h>
#include <string>

#include "ConsoleProvider.h"

namespace Core {

	class WinConsoleProvider : public ConsoleProvider
	{
	public:
		WinConsoleProvider();
		~WinConsoleProvider() = default;
	private:
		bool log(const std::string &message) override;
		bool logLine(const std::string &message) override;

		HANDLE _stdHandle;
	};

}

#endif // _WIN32
#endif // !WIN_CONSOLE_PROVIDER_H
