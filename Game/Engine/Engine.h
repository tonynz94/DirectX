#pragma once
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "Mesh.h"
#include "Shader.h"

class Engine
{
public:
	void Init(const WindowInfo& info); //초기화
	void Render(); //랜더링

public:
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; } //GPU 요청할 것들을 모아서 한번에 보냄
	shared_ptr<SwapChain> GetSwapChain(){ return _swapChain; }	//더블 버퍼링
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; }
	shared_ptr<ConstantBuffer> GetCB() { return _cb; }
	shared_ptr< TableDescriptorHeap> GetTableDescHeap() { return _tableDescHeap; }

public:
	void RenderBegin(); //CommandQueue에 일감을 주는 것
	void RenderEnd(); //쌓은 일감들을 GPU에 넘겨 줌(일감들이 그리기 시작함.)

	void ResizeWindow(int32 width, int32 height);
private:
	//그려질 화면 크기 관련
	WindowInfo			 _window;
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT		 _scissorRect = {};

	shared_ptr<Device> _device; //GPU에 요청을 위해 인력사무소 대표느낌
	shared_ptr<CommandQueue> _cmdQueue; //GPU 요청할 것들을 모아서 한번에 보냄
	shared_ptr<SwapChain> _swapChain;	//더블 버퍼링
	shared_ptr<RootSignature> _rootSignature;
	shared_ptr<ConstantBuffer> _cb;
	shared_ptr<TableDescriptorHeap> _tableDescHeap;

};

