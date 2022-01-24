#pragma once

class SwapChain;
class DescriptorHeap;

//여기에 요청사항을 넣어줌
//일감들을 몰아서 실행하는 클래스
class CommandQueue
{
public:
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	//실질적으로 그려주는 역할
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return _cmdList; }


private:
	// CommandQueue : DX12에 등장
	// 외주를 요청할 때, 하나씩 요청하면 비효율적
	// [외주 목록]에 일감을 차곡차곡 기록했다가 한 방에 요청하는 것
	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<ID3D12CommandAllocator>			_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>		_cmdList;

	// Fence : 울타리(?)
	// CPU / GPU 동기화를 위한 간단한 도구
	//TT 상대방이 외주를 주는 작업이 끝날때 까지 기다리겠다라고 할떄 Fence를 활용하여 기다림.
	//TT ex)지금까지 모아서 외주를 줬는데 105번이 다 끝나고 하겠다. 를 fenceValue를 이용 
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>		_swapChain;
};

