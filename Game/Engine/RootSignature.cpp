#include "pch.h"
#include "RootSignature.h"

void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	//레지스터를 사용하기 위해 계약을 맺는 행위
	//b~로 시작하는거는 상수버퍼용.
	CD3DX12_ROOT_PARAMETER param[2];
	param[0].InitAsConstantBufferView(0); // 0번 -> b0 -> CBV(constant buffer를 가리키는 포인터)
	param[1].InitAsConstantBufferView(1); // 1번 -> b1 -> CBV

	//기본상태의 시그니처를 만듬.
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(2, param);
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}
