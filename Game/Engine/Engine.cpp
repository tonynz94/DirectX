#include "pch.h"
#include "Engine.h"


void Engine::Init(const WindowInfo& info)
{
	_window = info;
	ResizeWindow(info.width, info.height);

	//그려질 화면 크기를 설정.
	_viewport = { 0,0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height),0.0f,1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();
	_cb = make_shared<ConstantBuffer>();
	_tableDescHeap = make_shared<TableDescriptorHeap>();

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init(_device->GetDevice());
	_cb->Init(sizeof(Transform), 256);	//256개의 버퍼를 만들어 줌
	_tableDescHeap->Init(256);
}

void Engine::Render()
{
	RenderBegin();

	// TODO : 나머지 물체들 그려준다

	RenderEnd();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	_window.width = width;
	_window.height = height;

	RECT rect = { 0 , 0, width, height };
	//::로시작하는건 우리가 정의한것이 아니라 라이브러리에서 제공해주는 것.
	//윈도우의 크기를 조절한다.
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	//원하는 위치에다가 가로,세로 높이를 설정하고 100,100에 위치시킴.
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);
}
