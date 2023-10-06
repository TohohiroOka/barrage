#include "Fog.h"

void Fog::CreateConstBuffer()
{
	// �萔�o�b�t�@�̐���
	HRESULT result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

}

void Fog::Update()
{
	// �萔�o�b�t�@�փf�[�^�]��
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->strength = strength;
		constMap->color = color;
		constBuff->Unmap(0, nullptr);
	}
}

Fog::Fog()
{
	//�t�H�O�̋���
	strength = 1.5f;
	color = { 1.0f,1.0f,1.0f };
}

std::unique_ptr<Fog> Fog::Create()
{
	// Sprite�̃C���X�^���X�𐶐�
	Fog* instance = new Fog();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize(EffectTyep::fog);
	//�萔�o�b�t�@
	instance->CreateConstBuffer();
	//�[�x�o�b�t�@����
	instance->CreateDepthBuffer();

	//���j�[�N�|�C���^��Ԃ�
	return std::unique_ptr<Fog>(instance);
}

void Fog::Draw(const Texture*_tex)
{
	Fog::Update();

	BasePostEffect::Draw(_tex);
}