#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

//��� ���۸� GPU�ʿ� ���ϴ� �����͸� ���縦 ���༭ �׷��޶�� �ϴ� ��

void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

//�׸��� �׷��޷ΰ� ��û�ϴ� �κ�
void Mesh::Render()
{
	//�並 �̿��ؼ� �ǳ��ְ� ����.
	//�������� �ﰢ���·� ���� ���ִ�.��� �����ִ� ��
	//vertesx ���۸� �Ѱ��ִ°� ����
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	//�ﰢ���� �׸����߶�� ��
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15) view�� ���ҽ��� �����ϰ� �ִ� �並 ���� �׷���.
	CMD_LIST->IASetIndexBuffer(&_indexBufferView);
	// TODO
	// 1) Buffer���ٰ� ������ ����
	// 2) Buffer�� �ּҸ� register���ٰ� ����
	//
	//GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
	//GEngine->GetCB()->PushData(1, &_transform, sizeof(_transform));

	// TODO
	// 1) Buffer���ٰ� ������ ����
	// 2) TableDescHeap���ٰ� CBV ����
	// 3) ��� ������ �������� TableDescHeap Ŀ��

	{
		//CPU ���ۿ��� -> GPU ���۷� ���縦 ����
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b0); //�������ִ°�
	}
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b1);
	}

	//���̺� ���
	GEngine->GetTableDescHeap()->CommitTable();

	//TT �׸��󿡼� Input assembler �� �־��ִ� �۾�.
	//CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);		//(���ؽ� ����)�׷��ִ� �۾�.
	CMD_LIST->DrawIndexedInstanced(_indexCount, 1, 0, 0, 0); //�ε��� ����
}

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	//����� ���� ������ ����. size 3��(���� ����)
	_vertexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);


	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize); //buffersize��ŭ GPU���� ��û�� ��.

	//���۸� GPU�ʿ� ũ�⸸ŭ �Ҵ��ϰ� ����.
	//���ҽ��� �������.
	DEVICE->CreateCommittedResource(
		&heapProperty,	//
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	//������ ������� ������ �츮�� ���ϴ� �Լ��� ��������
	//GPU ������ ����Ű�� ����
	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);	//Map�� ���� 
	::memcpy(vertexDataBuffer, &buffer[0], bufferSize);	//������ ���, ������ �ּ� ���۰�, ũ��
	_vertexBuffer->Unmap(0, nullptr); //GPU�ʿ� �޸𸮰� ����Ǵ� ����

	// Initialize the vertex buffer view.
	//���۸� �������ִ� view�� ����� �� ������
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // ���� 1�� ũ��
	_vertexBufferView.SizeInBytes = bufferSize; // ������ ũ��	
}
//TT => _vertexBufferView�� ���� �������� GPU�� ����

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	
	_indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _indexCount * sizeof(uint32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_indexBuffer));

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_indexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	_indexBuffer->Unmap(0, nullptr);

	_indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
	_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	_indexBufferView.SizeInBytes = bufferSize;
}