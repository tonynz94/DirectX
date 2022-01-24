#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec)
{
	//벡터 정보를 받음.
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);


	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize); //buffersize만큼 GPU에게 요청을 함.

	//GPU쪽에 가서 공간을 할당하고 있음.
	//리소스를 만들어줌.
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	//GPU 공간을 가리키고 있음
	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);	//Map를 통해 
	::memcpy(vertexDataBuffer, &vec[0], bufferSize);
	_vertexBuffer->Unmap(0, nullptr); //GPU쪽에 메모리가 복사되는 행위

	// Initialize the vertex buffer view.
	//버퍼를 설명해주는 view를 만들어 준 다음에
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}

//그려지는 부분
void Mesh::Render()
{
	//뷰를 이용해서 건네주고 있음.
	//정점들이 삼각형태로 묘사 되있다.라고 말해주는 것
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15) view는 리소스를 묘사하고 있는 뷰를 통해 그려줌.
	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);		//그려주는 작업.
}