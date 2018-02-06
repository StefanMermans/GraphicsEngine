#ifndef DIRECT_X_ENGINE_H
#define DIRECT_X_ENGINE_H

#include "..\stdafx.h"

#include "DirectXController.h"
#include "WindowController.h"

class DirectXEngine
{
public:
	DirectXEngine();
	~DirectXEngine();

	bool init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow);
	bool update();

	int mainLoop();
private:
	DirectXController * _dxControllerPtr;
	WindowController * _windowControllerPtr;
};

#endif // !DIRECT_X_ENGINE_H
