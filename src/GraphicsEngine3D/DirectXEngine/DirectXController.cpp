#include "..\stdafx.h"
#include "DirectXController.h"
#include "DirectXUtils.h"
#include "ShaderBuilder.h"
#include "..\Core\Vertex.h"
#include <glm.hpp>

#include <d3dcompiler.h>

using DXUtils::throwIfFailed;
using Core::Vertex;

DirectXController::DirectXController(const WindowDefinition & windowDef, DxEventHandler * handlerPtr)
{
	_windowDef = windowDef;
	_dxEventHandlerPtr = handlerPtr;

	_viewPort = {
		0.0f,
		0.0f,
		static_cast<float>(_windowDef.width),
		static_cast<float>(_windowDef.height),
		D3D12_MIN_DEPTH,
		D3D12_MAX_DEPTH
	};

	_scissorRect = {
		0,
		0,
		static_cast<LONG>(_windowDef.width),
		static_cast<LONG>(_windowDef.height)
	};
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
	populateCommandList();

	// Execute any command lists
	ID3D12CommandList* commandListsPtr[] = { _commandListPtr };
	_commandQueuePtr->ExecuteCommandLists(_countof(commandListsPtr), commandListsPtr);

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

	createVertexResource();

	waitForPreviousFrame();
}

void DirectXController::createVertexResource() {
	// Create the vertex buffer.
	{
		// Define the geometry for a triangle.
		float aspectRatio = _windowDef.aspectRatio();
		Vertex triangleVertices[] =
		{
			{ { 0.0f, 0.25f * aspectRatio, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f * aspectRatio, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f * aspectRatio, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices);

		D3D12_HEAP_PROPERTIES props{};
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.CreationNodeMask = 1;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		props.VisibleNodeMask = 1;
		props.Type = D3D12_HEAP_TYPE_UPLOAD;

		D3D12_RESOURCE_DESC desc{};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment = 0;
		desc.Width = vertexBufferSize;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		throwIfFailed(_devicePtr->CreateCommittedResource(
			&props,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_vertexBuferPtr)));

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin;
		D3D12_RANGE readRange{ 0, 0 };		// We do not intend to read from this resource on the CPU.
		throwIfFailed(_vertexBuferPtr->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		_vertexBuferPtr->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		_vertexBufferView.BufferLocation = _vertexBuferPtr->GetGPUVirtualAddress();
		_vertexBufferView.StrideInBytes = sizeof(Vertex);
		_vertexBufferView.SizeInBytes = vertexBufferSize;
	}
}

void DirectXController::populateCommandList()
{
	throwIfFailed(_commandAllocatorPtr->Reset());

	throwIfFailed(_commandListPtr->Reset(_commandAllocatorPtr, _basePipelineStatePtr));

	_commandListPtr->SetGraphicsRootSignature(_rootSignaturePtr);
	_commandListPtr->RSSetViewports(1, &_viewPort);
	_commandListPtr->RSSetScissorRects(1, &_scissorRect);
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
	ID3DBlob * vertexShaderPtr = NULL;
	ID3DBlob * pixelShaderPtr = NULL;

#ifdef _DEBUG
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif // _DEBUG

	std::string shaderPath = "Assets/basicShader.hlsl";

	bool success = ShaderBuilder::buildShader(shaderPath, nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, 0, &vertexShaderPtr, nullptr);
	success = success && ShaderBuilder::buildShader(shaderPath, nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, 0, &pixelShaderPtr, nullptr);
	if (!success) {
		// TODO: make this prettier.
		throw std::exception();
	}


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
