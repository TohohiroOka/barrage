#include "PlayerSword.h"

using namespace DirectX;

PlayerSword::PlayerSword(Fbx* parent)
{
	//������������e���Z�b�g
	this->parent = parent;

	//�����f���ǂݍ���
	swordModel = Model::CreateFromOBJ("sword");
	std::string bone = "thumb.01.R";
	XMMATRIX matScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
	matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));
	DirectX::XMMATRIX matTrans = XMMatrixTranslation(0.0f, -0.15f, 0.0f);
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world *= matScale;
	world *= matRot;
	world *= matTrans;
	parent->SetBoneObject(bone, "rightHand", swordModel.get(), world, true, "sword1");

	//���̒����}��p
	{
		XMMATRIX matScale = XMMatrixScaling(0.0f, 0.0f, 0.0f);
		XMMATRIX matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
		matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
		matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));
		DirectX::XMMATRIX matTrans = XMMatrixTranslation(0.0f, 1.5f, 0.0f);
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		world *= matScale;
		world *= matRot;
		world *= matTrans;
		parent->SetBoneObject(bone, "rightHand", swordModel.get(), world, false, "sword2");
	}

	//�a�����o����
	XMFLOAT4 startColor = { 0.02f, 0.05f, 0.2f, 1.0f };
	XMFLOAT4 endColor = { 0.001f, 0.001f, 0.01f, 1.0f };
	slashEffect = std::make_unique<SlashEffect>("effect", 10, 10, 10.0f, 1.0f, 0.0f, startColor, endColor, false);
}

PlayerSword::~PlayerSword()
{
}

void PlayerSword::Update()
{
	//�a���G�t�F�N�g�X�V
	slashEffect->Update(parent->GetAttachPos("sword1"), parent->GetAttachPos("sword2"));
}

void PlayerSword::DrawEffect()
{
	//�a���G�t�F�N�g�`��
	slashEffect->Draw();
}
