#include "Base3D.h"
#include "Object/3d/collider/BaseCollider.h"
#include "Object/3d/collider/CollisionManager.h"
#include "Camera/Camera.h"
#include "Camera/LightCamera.h"
#include "Light/LightGroup.h"
#include "WindowApp.h"

using namespace DirectX;

Camera* Base3D::camera = nullptr;
LightCamera* Base3D::lightCamera = nullptr;
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
		//�e�I�u�W�F�N�g�̃��[���h�s���������
		matWorld *= parent->GetMatWorld();
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

const XMFLOAT2 Base3D::GetScreenPosition()
{
	XMFLOAT3 worldPos = GetWorldPosition();

	//�r���[�|�[�g�s��
	XMMATRIX matViewport = {
		{(float)WindowApp::GetWindowWidth() / 2, 0, 0, 0 },
		{0, -(float)WindowApp::GetWindowHeight() / 2, 0, 0},
		{0, 0, 1, 0},
		{(float)WindowApp::GetWindowWidth() / 2, (float)WindowApp::GetWindowHeight() / 2, 0, 1}
	};

	//�r���[�s��ƃv���W�F�N�V�����s��ƃr���[�|�[�g�s�������
	XMMATRIX matViewProjectionViewport =
		camera->GetView() * camera->GetProjection() * matViewport;

	//���[���h���W����X�N���[�����W�ɕϊ�
	worldPos = MatrixTransformWDivision(worldPos, matViewProjectionViewport);
	XMFLOAT2 screenPos = { worldPos.x, worldPos.y };

	return screenPos;
}

void Base3D::ColliderDraw()
{
	collider->Draw();
}
