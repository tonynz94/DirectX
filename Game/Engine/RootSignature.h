#pragma once

//계약서 / 결재

// CPU[        ]    GPU[        ]
// 한국[        ] 베트남[        ]
//CPU가 GPU에게 외주를 주고 있음.
//GPU도 CPU에 있는 RAM메모리안에 있는 데이터가 필요로함. 근데 매번 가져오면 너무 과부하기 심함.

class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	//comPtr 대부분은 GPU에게 요청하는 것
	ComPtr<ID3D12RootSignature> _signature;


};

