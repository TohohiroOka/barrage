#include "Base3D.h"
#include "Object/3d/collider/BaseCollider.h"
#include "Object/3d/collider/CollisionManager.h"
#include "Camera/Camera.h"
#include "Light/LightGroup.h"

using namespace DirectX;

Camera* Base3D::camera = nullptr;
LightGroup* Base3D::light = nullptr;

void Base3D::Update()
{
	// �����蔻��X�V
	if (collider) {
		collider->Update();
	}
}

void Base3D::UpdateWorldMatrix()
{
	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
	matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	// �e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		XMFLOAT3 pPos = parent->GetPosition();
		XMFLOAT3 pRota = parent->GetRotation();
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= XMMatrixTranslation(-pPos.x, -pPos.y, -pPos.z);
		matWorld *= XMMatrixRotationZ(XMConvertToRadians(pRota.z));
		matWorld *= XMMatrixRotationZ(XMConvertToRadians(pRota.x));
		matWorld *= XMMatrixRotationZ(XMConvertToRadians(pRota.y));
		matWorld *= XMMatrixTranslation(pPos.x, pPos.y, pPos.z);

		//XMFLOAT3 radiun = { asinf(-matWorld.r[2].m128_f32[1]),
		//	asinf(-matWorld.r[0].m128_f32[2]),asinf(-matWorld.r[1].m128_f32[0]) };
		//rotation = { XMConvertToDegrees(radiun.x),XMConvertToDegrees(radiun.y) ,XMConvertToDegrees(radiun.z) };
		//position = { matWorld.r[3].m128_f32[0],matWorld.r[3].m128_f32[1],matWorld.r[3].m128_f32[2] };

		//rotation = { rotation.x + pRota.x,rotation.y + pRota.y, rotation.z + pRota.z };

		//matWorld *= parent->GetMatWorld();
	}
}

void Base3D::DeleteCollider()
{
	//�R���C�_�[���
	if (collider) {
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

void Base3D::SetCollider(BaseCollider* _collider)
{
	_collider->SetObject(this);
	this->collider = _collider;
	// �R���W�����}�l�[�W���ɒǉ�
	CollisionManager::GetInstance()->AddCollider(_collider);

	UpdateWorldMatrix();
	_collider->Update();
}

const XMFLOAT3 Base3D::GetWorldPosition()
{
	XMFLOAT3 worldpos;

	worldpos.x = matWorld.r[3].m128_f32[0];
	worldpos.y = matWorld.r[3].m128_f32[1];
	worldpos.z = matWorld.r[3].m128_f32[2];

	return worldpos;
}

void Base3D::ColliderDraw()
{
	collider->Draw();
}
