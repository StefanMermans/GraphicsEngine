#ifndef DIRECT_X_CONTROLLER_H
#define DIRECT_X_CONTROLLER_H

#include "..\stdafx.h"

#include "DxEventHandler.h"

struct WindowDefinition {
	UINT width;
	UINT height;
	std::string title;
	float aspectRatio() { // This is kinda ugly...
		return static_cast<float>(width) / static_cast<float>(height);
	}
};

class DirectXController
{
public:
	DirectXController(const WindowDefinition& windowDef, DxEventHandler * handlerPtr);
	DirectXController(UINT width, UINT height, std::string title);
	~DirectXController();

	void init(HWND * hwndPtr);

	// Event targets
	void update();
	void draw();
	void destroy();
	void keyDown(UINT8 key);
	void keyUp(UINT8 key);

	// Getters
	inline UINT getWidth() const { return _windowDef.width; };
	inline UINT getHeight() const { return _windowDef.height; };
	inline std::string getTitle() const { return _windowDef.title; };
private:
	void loadDXpipeLine(HWND * pHwnd);
	void waitForPreviousFrame();
	void createBasicPipelineState();
	void createVertexResource();
	void populateCommandList();


	DxEventHandler * _dxEventHandlerPtr;

	// The amount of frames with wich can be swapped (2 means 1 buffered, and 1 displayed)
	static const UINT frameCount = 2; 

	// Window definition
	WindowDefinition _windowDef;

	// -------------------
	// Pipeline members
	ID3D12PipelineState * _basePipelineStatePtr;

	D3D12_VIEWPORT _viewPort;
	D3D12_RECT _scissorRect;

	ID3D12Device * _devicePtr;
	ID3D12Resource * _pRenderTargets[frameCount];
	ID3D12RootSignature * _rootSignaturePtr;

	// Fence
	ID3D12Fence * _fencePtr;
	UINT64 _fenceValue;
	HANDLE _fenceEvent;

	// render target view (RTV) descriptor heap.
	ID3D12DescriptorHeap * _rtvHeapPtr;
	UINT _rtvDescriptorSize;

	// Swap chain
	IDXGISwapChain3 * _swapChainPtr;
	UINT _frameIndex;

	// Command queue
	ID3D12GraphicsCommandList1 * _commandListPtr;
	ID3D12CommandQueue * _commandQueuePtr;
	ID3D12CommandAllocator * _commandAllocatorPtr;

	// Vertex stuff / This should be moved to a seperate class later
	ID3D12Resource * _vertexBuferPtr;
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;

	// -------------------
};

#endif // !DIRECT_X_CONTROLLER_H

