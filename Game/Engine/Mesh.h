#pragma once

// [����Ƽ¯]�� ���� �������� �̷���� ��ü
class Mesh
{
public:

private:
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;
};

