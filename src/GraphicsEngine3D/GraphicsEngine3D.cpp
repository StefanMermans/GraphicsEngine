#include "stdafx.h"
#include "DirectxEngine\DirectXEngine.h"
#include "Core\EngineCore.h"
#include "OpenGLEngine\OpenGLEngine.h"

using Core::Logger;

#ifdef ENGINE_USE_DX

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
#ifdef _DEBUG
	Logger::init();
#endif // _DEBUG
	Logger::logLine("Running DirectX engine...");

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

#else // ENGINE_USE_DX

int main(int argc, char * argv[]) {
#ifdef _DEBUG
	Logger::init(true, true);
#endif // _DEBUG
	Logger::logLine("Running OpenGL only...");

	OpenGLEngine engine{ argc, argv };

	return engine.run();
}

#endif // ENGINE_USE_DX
