#ifndef DX_EVENT_HANDLER_H
#define DX_EVENT_HANDLER_H

class DxEventHandler {
public:
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void destroy() = 0;
	virtual void keyDown(UINT8 key) = 0;
	virtual void keyUp(UINT8 key) = 0;
};

#endif // !DX_EVENT_HANDLER_H

