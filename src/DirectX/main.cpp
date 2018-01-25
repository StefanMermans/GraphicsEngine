#ifdef _WIN32

#include "stdafx.h"
#include "EngineCore.h"
#include "DirectXEngine.h"

using Core::Logger;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
#ifdef _DEBUG
	Logger::init();
#endif // _DEBUG
	Logger::logLine("Running DirectX project...");

	DirectXEngine engine{};

	if (!engine.init(hInstance, hPrevInstance, lpCmdLine, nCmdShow)) {
		return -1;
	}

	// Main loop
	while (true) {
		// Update the WindowController and get the continue status
		bool running = engine.update();

		if (!running) {
			break;
		}
	}

	return 0;
}

#else 

int main() {

	return 0;
}

#endif // _WIN32
