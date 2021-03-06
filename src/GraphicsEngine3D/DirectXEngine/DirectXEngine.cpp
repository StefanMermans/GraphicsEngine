#include "..\stdafx.h"
#include "DirectXEngine.h"


DirectXEngine::DirectXEngine()
{
	_dxControllerPtr = new DirectXController({ 1280, 720, "DirectX" }, nullptr);
	_windowControllerPtr = NULL;
}

DirectXEngine::~DirectXEngine()
{
}

bool DirectXEngine::init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	// Initialise the WindowController.
	_windowControllerPtr = new WindowController(hInstance, nCmdShow);
	return _windowControllerPtr->init(_dxControllerPtr);
}

bool DirectXEngine::update()
{
	return _windowControllerPtr->update();
}

int DirectXEngine::mainLoop()
{
	// Main loop
	while (true) {
		// Update the WindowController and get the continue status
		bool running = update();

		if (!running) {
			break;
		}
	}

	return 0;
}
