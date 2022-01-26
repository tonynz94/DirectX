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
	void Init(const WindowInfo& info); //�ʱ�ȭ
	void Render(); //������

public:
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; } //GPU ��û�� �͵��� ��Ƽ� �ѹ��� ����
	shared_ptr<SwapChain> GetSwapChain(){ return _swapChain; }	//���� ���۸�
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; }
	shared_ptr<ConstantBuffer> GetCB() { return _cb; }
	shared_ptr< TableDescriptorHeap> GetTableDescHeap() { return _tableDescHeap; }

public:
	void RenderBegin(); //CommandQueue�� �ϰ��� �ִ� ��
	void RenderEnd(); //���� �ϰ����� GPU�� �Ѱ� ��(�ϰ����� �׸��� ������.)

	void ResizeWindow(int32 width, int32 height);
private:
	//�׷��� ȭ�� ũ�� ����
	WindowInfo			 _window;
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT		 _scissorRect = {};

	shared_ptr<Device> _device; //GPU�� ��û�� ���� �η»繫�� ��ǥ����
	shared_ptr<CommandQueue> _cmdQueue; //GPU ��û�� �͵��� ��Ƽ� �ѹ��� ����
	shared_ptr<SwapChain> _swapChain;	//���� ���۸�
	shared_ptr<RootSignature> _rootSignature;
	shared_ptr<ConstantBuffer> _cb;
	shared_ptr<TableDescriptorHeap> _tableDescHeap;

};

