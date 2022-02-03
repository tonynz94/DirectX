#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

//어떠한 버퍼를 GPU쪽에 원하는 데이터를 복사를 해줘서 그려달라고 하는 곳
void Mesh::Init(vector<Vertex>& vec)
{
	//삼격형 벡터 정보를 받음. size 3개(정점 갯수)
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);	 


	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize); //buffersize만큼 GPU에게 요청을 함.

	//버퍼를 GPU쪽에 가서 할당하고 있음.
	//리소스를 만들어줌.
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	//위에서 만들어준 공간에 우리가 원하는 함수를 복사해줌
	//GPU 공간을 가리키고 있음
	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);	//Map를 통해 
	::memcpy(vertexDataBuffer, &vec[0], bufferSize);	//복사할 장소, 복사할 주소 시작값, 크기
	_vertexBuffer->Unmap(0, nullptr); //GPU쪽에 메모리가 복사되는 행위

	// Initialize the vertex buffer view.
	//버퍼를 설명해주는 view를 만들어 준 다음에
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}
//TT => _vertexBufferView에 정점 정보들이 GPU에 들어갔음


//그리고 그려달로고 요청하는 부분
void Mesh::Render()
{
	//뷰를 이용해서 건네주고 있음.
	//정점들이 삼각형태로 묘사 되있다.라고 말해주는 것
	//vertesx 버퍼를 넘겨주는건 쉬움
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15) view는 리소스를 묘사하고 있는 뷰를 통해 그려줌.
	
	// TODO
	// 1) Buffer에다가 데이터 세팅
	// 2) Buffer의 주소를 register에다가 전송
	//
	//GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
	//GEngine->GetCB()->PushData(1, &_transform, sizeof(_transform));

	// TODO
	// 1) Buffer에다가 데이터 세팅
	// 2) TableDescHeap에다가 CBV 전달
	// 3) 모두 세팅이 끝났으면 TableDescHeap 커밋

	{
		//CPU 버퍼에서 -> GPU 버퍼로 복사를 해줌
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b0); //복사해주는것
	}
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b1);
	}

	//테이블 사용
	GEngine->GetTableDescHeap()->CommitTable();

	//TT 그림상에서 Input assembler 에 넣어주는 작업.
	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);		//그려주는 작업.
}