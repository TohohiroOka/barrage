#include "Depth.h"
#include "WindowApp.h"

using namespace DirectX;

std::unique_ptr<Depth> Depth::Create(const std::string& _texName)
{
	//�C���X�^���X�𐶐�
	Depth* instance = new Depth();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize(_texName);

	//���j�[�N�|�C���^��Ԃ�
	return std::unique_ptr<Depth>(instance);
}

void Depth::Initialize(const std::string& _texName)
{
	// �e�N�X�`���p�o�b�t�@�̐���
	texture = std::make_unique<TextureManager>(_texName);
}

void Depth::PreDrawScene()
{
	//���\�[�X�o���A��ύX(�V�F�[�_���\�[�X���`��\)
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(texture->GetTexBuffer(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = texture->GetDescriptor()->GetDsvHandle();

	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(0, nullptr, false, &dsvH);

	std::array<UINT, 2> size = texture->GetSize();

	//�r���[�|�[�g�̐ݒ�
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		FLOAT(size[0]), FLOAT(size[1])));
	//�V�U�����O��`�̐ݒ�
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0,
		LONG(size[0]), LONG(size[1])));

	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0,
		nullptr);
}

void Depth::PostDrawScene()
{
	//���\�[�X�o���A��ύX(�`��\���V�F�[�_���\�[�X)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture->GetTexBuffer(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ));
}
