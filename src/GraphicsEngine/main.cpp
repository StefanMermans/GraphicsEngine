#ifdef _WIN32
#include "stdafx.h"

#include <Windows.h>

#include "EngineCore.h"
#include "DirectXEngine.h"
#include <iostream>

using Core::Logger;

// True program entry point
int run(DirectXEngine engine) {
#ifdef _DEBUG
	Logger::init();
#endif // _DEBUG
	Logger::logLine("Starting the engine!");

	while (true)
	{
		bool running = engine.update();

		if (!running) {
			break;
		}
	}

	return 0;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	DirectXEngine engine{};

	if (!engine.init(hInstance, hPrevInstance, lpCmdLine, nCmdShow)) {
		return -1;
	}

	return run(engine);
}
#else
#include <iostream>

int main(int argc, char * argv[]) {
	std::cout << "operating systems other than windows are not yet supported!" << std::endl;
	
	return 0;
}

#endif // _WIN32
