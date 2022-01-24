#pragma once

//��༭ / ����

// CPU[        ]    GPU[        ]
// �ѱ�[        ] ��Ʈ��[        ]
//CPU�� GPU���� ���ָ� �ְ� ����.
//GPU�� CPU�� �ִ� RAM�޸𸮾ȿ� �ִ� �����Ͱ� �ʿ����. �ٵ� �Ź� �������� �ʹ� �����ϱ� ����.

class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	//comPtr ��κ��� GPU���� ��û�ϴ� ��
	ComPtr<ID3D12RootSignature> _signature;


};

