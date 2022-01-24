#pragma once

// [유니티짱]과 같은 정점으로 이루어진 물체
class Mesh
{
public:

private:
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;
};

