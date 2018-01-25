#include "stdafx.h"
#include "DirectXController.h"
#include "DirectXUtils.h"

#include <d3dcompiler.h>

using DXUtils::throwIfFailed;

DirectXController::DirectXController(const WindowDefinition & windowDef, DxEventHandler * handlerPtr)
{
	_windowDef = windowDef;
	_dxEventHandlerPtr = handlerPtr;
}

DirectXController::DirectXController(UINT width, UINT height, std::string title)
{
	_windowDef.width = width;
	_windowDef.height = height;
	_windowDef.title = title;
}

DirectXController::~DirectXController()
{
}

void DirectXController::init(HWND * hwndPtr) {
	loadDXpipeLine(hwndPtr);

	if (_dxEventHandlerPtr) {
		_dxEventHandlerPtr->draw();
	}
}

void DirectXController::update()
{
	if (_dxEventHandlerPtr) {
		_dxEventHandlerPtr->update();
	}
}

void DirectXController::draw()
{
	// Command list initialisation before calling handler
	throwIfFailed(_commandAllocatorPtr->Reset());

	// Calling the handler's draw function, allowing it to do it's draw step.
	if (_dxEventHandlerPtr) {
		_dxEventHandlerPtr->draw();
	}

	// Swap buffers and wait for the previous frame
	throwIfFailed(_swapChainPtr->Present(1, 0));
	waitForPreviousFrame();
}

void DirectXController::destroy()
{
	waitForPreviousFrame();

	CloseHandle(_fenceEvent);

	if (_dxEventHandlerPtr) {
		_dxEventHandlerPtr->destroy();
	}
}

void DirectXController::keyDown(UINT8 key)
{
	if (_dxEventHandlerPtr) {
		_dxEventHandlerPtr->keyDown(key);
	}
}

void DirectXController::keyUp(UINT8 key)
{
	if (_dxEventHandlerPtr) {
		_dxEventHandlerPtr->keyUp(key);
	}
}

void DirectXController::loadDXpipeLine(HWND * hwndPtr)
{
	UINT8 dxgiFactoryFlags = 0;

#ifdef _DEBUG
	// Create debugController
	{
		ID3D12Debug * debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();

			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
		else {
			throw std::exception("Failed to initialise the debugController!");
		}
	}
#endif // _DEBUG

	// Create IDXGIFactory
	IDXGIFactory4 * factory4;
	throwIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory4)));

	// Create hardwareAdapter (warpHardwareAdapter)
	IDXGIAdapter * warpHardwareAdapter;
	throwIfFailed(factory4->EnumWarpAdapter(IID_PPV_ARGS(&warpHardwareAdapter)));

	throwIfFailed(D3D12CreateDevice(
		warpHardwareAdapter,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&_devicePtr)
	));

	// Create command queue
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	throwIfFailed(_devicePtr->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_commandQueuePtr)));

	// Create swap chain
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = frameCount;
	swapChainDesc.Width = _windowDef.width;
	swapChainDesc.Height = _windowDef.height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	IDXGISwapChain1 * swapChain;
	throwIfFailed(factory4->CreateSwapChainForHwnd(
		_commandQueuePtr,
		*hwndPtr,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	));

	throwIfFailed(factory4->MakeWindowAssociation(*hwndPtr, DXGI_MWA_NO_ALT_ENTER));

	_swapChainPtr = static_cast<IDXGISwapChain3*>(swapChain);
	_frameIndex = _swapChainPtr->GetCurrentBackBufferIndex();

	// Create descriptor heaps.
	{
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = frameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		throwIfFailed(_devicePtr->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeapPtr)));

		_rtvDescriptorSize = _devicePtr->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// Create frame resources.
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeapPtr->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame (frame buffer?)
		for (UINT i = 0; i < frameCount; i++) {
			throwIfFailed(_swapChainPtr->GetBuffer(i, IID_PPV_ARGS(&_pRenderTargets[i])));
			_devicePtr->CreateRenderTargetView(_pRenderTargets[i], NULL, rtvHandle);
			rtvHandle.ptr += 1 * _rtvDescriptorSize; // This weird thing replaces Microsoft's custom CD3DX12_CPU_DESCRIPTOR_HANDLE
		}
	}

	throwIfFailed(_devicePtr->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocatorPtr)));

	// Create synchronization objects and wait until assets have been uploaded to the GPU.
	{
		throwIfFailed(_devicePtr->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fencePtr)));
		_fenceValue = 1;

		// Create an event handle to use for frame synchronization.
		_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (_fenceEvent == nullptr)
		{
			throwIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}

		// Wait for the command list to execute; we are reusing the same command 
		// list in our main loop but for now, we just want to wait for setup to 
		// complete before continuing.
		waitForPreviousFrame();
	}

	// Create an empty root signaturePtr.
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.NumParameters = 0;
	rootSignatureDesc.pParameters = nullptr;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob * signaturePtr;
	ID3DBlob * errorPtr;
	throwIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signaturePtr, &errorPtr));
	throwIfFailed(_devicePtr->CreateRootSignature(0, signaturePtr->GetBufferPointer(), signaturePtr->GetBufferSize(), IID_PPV_ARGS(&_rootSignaturePtr)));

	// Create the basic pipeline state
	createBasicPipelineState();

	// Create the command list
	throwIfFailed(_devicePtr->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocatorPtr, _basePipelineStatePtr, IID_PPV_ARGS(&_commandListPtr)));
	throwIfFailed(_commandListPtr->Close());
}

void DirectXController::waitForPreviousFrame()
{
	const UINT64 fence = _fenceValue;
	throwIfFailed(_commandQueuePtr->Signal(_fencePtr, fence));
	_fenceValue++;

	if (_fencePtr->GetCompletedValue() < fence) {
		throwIfFailed(_fencePtr->SetEventOnCompletion(fence, _fenceEvent));
		WaitForSingleObject(_fenceEvent, INFINITE);
	}

	_frameIndex = _swapChainPtr->GetCurrentBackBufferIndex();
}

void DirectXController::createBasicPipelineState()
{
	// Create the basic pipeline state.
	ID3DBlob * vertexShaderPtr;
	ID3DBlob * pixelShaderPtr;

#ifdef _DEBUG
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif // _DEBUG

	std::wstring shaderPath = L"Assets/shaders.hlsl"; // IDK what's up with this string type... SEND HELP!
	throwIfFailed(D3DCompileFromFile(shaderPath.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShaderPtr, nullptr));
	throwIfFailed(D3DCompileFromFile(shaderPath.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShaderPtr, nullptr));

	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// Get the bytecode of the vertex and pixel shaders
	D3D12_SHADER_BYTECODE vertexShaderBytes = { 0 };
	vertexShaderBytes.pShaderBytecode = vertexShaderPtr->GetBufferPointer();
	vertexShaderBytes.BytecodeLength = vertexShaderPtr->GetBufferSize();

	D3D12_SHADER_BYTECODE pixelShaderBytes = { 0 };
	pixelShaderBytes.pShaderBytecode = pixelShaderPtr->GetBufferPointer();
	pixelShaderBytes.BytecodeLength = pixelShaderPtr->GetBufferSize();

	// Create a rasterizer description
	D3D12_RASTERIZER_DESC rasterizer = {};
	rasterizer.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizer.CullMode = D3D12_CULL_MODE_BACK;
	rasterizer.FrontCounterClockwise = FALSE;
	rasterizer.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizer.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizer.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizer.DepthClipEnable = TRUE;
	rasterizer.MultisampleEnable = FALSE;
	rasterizer.AntialiasedLineEnable = FALSE;
	rasterizer.ForcedSampleCount = 0;
	rasterizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	// Create a blend description
	D3D12_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
	{
		FALSE,FALSE,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL,
	};
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
		blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;
	}

	// Actually create the basic pipline state.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = _rootSignaturePtr;
	psoDesc.VS = vertexShaderBytes;
	psoDesc.PS = pixelShaderBytes;
	psoDesc.RasterizerState = rasterizer;
	psoDesc.BlendState = blendDesc;
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	throwIfFailed(_devicePtr->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&_basePipelineStatePtr)));
}
