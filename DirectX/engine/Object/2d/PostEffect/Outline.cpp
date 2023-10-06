#include "Outline.h"

void Outline::CreateConstBuffer()
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

void Outline::Update()
{
	// �萔�o�b�t�@�փf�[�^�]��
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->outlineWidth = outlineWidth;
		constBuff->Unmap(0, nullptr);
	}
}

Outline::Outline()
	:BasePostEffect()
{
	//�A�E�g���C���̑���������
	outlineWidth = 1.0f;
}

std::unique_ptr<Outline> Outline::Create()
{
	// Sprite�̃C���X�^���X�𐶐�
	Outline* instance = new Outline();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize(EffectTyep::outline);
	//�萔�o�b�t�@
	instance->CreateConstBuffer();
	//�[�x�o�b�t�@����
	instance->CreateDepthBuffer();

	//���j�[�N�|�C���^��Ԃ�
	return std::unique_ptr<Outline>(instance);
}

void Outline::Draw(const Texture* _tex)
{
	Update();

	BasePostEffect::Draw(_tex);
}