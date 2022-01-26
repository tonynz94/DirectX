#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (_cbvBuffer)
	{
		if (_cbvBuffer != nullptr)
			_cbvBuffer->Unmap(0, nullptr);

		_cbvBuffer = nullptr;
	}
}



void ConstantBuffer::Init(uint32 size, uint32 count)
{
	// ��� ���۴� 256 ����Ʈ ����� ������ �Ѵ�
	// 0 256 512 768
	_elementSize = (size + 255) & ~255;	//������۴� 255 ����� ����� ���� ����
	_elementCount = count;

	CreateBuffer();
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;	//�ѹ濡 �������� ���۸� ��������
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//GPU�ʿ� ���۸� ����� �ִ� ��
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer));

	//Map�� �ؼ� Cpu�� ���縦�ؼ� �ݾ��ְ�  GPU�� ��(?)
	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

//���۸� ���� ���� �ٽ� 0�� ���ۺ��� �����͸� �־���
void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

//memcpy�� ���� cpu�������Ϳ� ������ �� �� GPU �������Ϳ� ���縦 �ϰ� b0,b1���� �ּҸ� �Ѱ��ִ� ����

void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	assert(_currentIndex < _elementSize);

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	//b0, b1�� ���۸� �Ѱ��ִ� ����
	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);

	//bo,b1���� �÷��� �̰� �θ��� ���� shader���� b0,b1�� ����� �� ����.   
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	_currentIndex++;	//���۸� ��ĭ�� �̷��ָ鼭 �����͸� ���ְ� ����
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;
	return objCBAddress;
}