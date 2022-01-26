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
	// 상수 버퍼는 256 바이트 배수로 만들어야 한다
	// 0 256 512 768
	_elementSize = (size + 255) & ~255;	//상수버퍼는 255 배수로 만들기 위한 공식
	_elementCount = count;

	CreateBuffer();
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;	//한방에 여러개의 버퍼를 만들어야함
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//GPU쪽에 버퍼를 만들어 주는 것
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer));

	//Map를 해서 Cpu에 복사를해서 닫아주고  GPU로 들어감(?)
	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

//버퍼를 새로 쓸대 다시 0번 버퍼부터 데이터를 넣어줌
void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

//memcpy를 통해 cpu레지스터에 저장을 한 후 GPU 레지스터에 복사를 하고 b0,b1으로 주소를 넘겨주는 행위

void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	assert(_currentIndex < _elementSize);

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	//b0, b1에 버퍼를 넘겨주는 행위
	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);

	//bo,b1으로 올려줌 이걸 부르면 이제 shader에서 b0,b1을 사용할 수 있음.   
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	_currentIndex++;	//버퍼를 한칸씩 미뤄주면서 데이터를 써주고 있음
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;
	return objCBAddress;
}