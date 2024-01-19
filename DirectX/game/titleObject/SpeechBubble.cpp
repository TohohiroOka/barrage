#include "SpeechBubble.h"
#include "engine/Camera/Camera.h"

SpeechBubble::SpeechBubble(Camera* camera, const std::string& modelName, const DirectX::XMFLOAT3& position, float scale)
{
	//�J�������Z�b�g
	this->camera = camera;

	//���f���ǂݍ���
	model = Model::CreateFromOBJ(modelName);

	//�I�u�W�F�N�g����
	object = Object3d::Create(model.get());
	object->SetCalcWorldMat(true);
	object->SetPosition(position);
	object->SetScale({ scale, scale, scale });
}

void SpeechBubble::Update()
{
	//Y���r���{�[�h�s��ōX�V����
	//���_���W
	DirectX::XMVECTOR eyePosition = XMLoadFloat3(&camera->GetEye());
	//�����_���W
	DirectX::XMVECTOR targetPosition = XMLoadFloat3(&camera->GetTarget());
	//���̏����(�Q�[���J�����͈��]���\�ɂ����邽�ߎ������炵�Ă���B�����Y���r���{�[�h�p�ɉ��̐^�㎲���K�v)
	DirectX::XMFLOAT3 baseUp(0, 1, 0);
	DirectX::XMVECTOR upVector = XMLoadFloat3(&baseUp);

	//�J����Z��(��������)
	DirectX::XMVECTOR cameraAxisZ = DirectX::XMVectorSubtract(targetPosition, eyePosition);

	//0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!DirectX::XMVector3Equal(cameraAxisZ, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(cameraAxisZ));
	assert(!DirectX::XMVector3Equal(upVector, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(upVector));

	//�x�N�g���𐳋K��
	cameraAxisZ = DirectX::XMVector3Normalize(cameraAxisZ);

	//�J������X��(�E����)
	DirectX::XMVECTOR cameraAxisX;
	//X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = DirectX::XMVector3Cross(upVector, cameraAxisZ);
	//�x�N�g���𐳋K��
	cameraAxisX = DirectX::XMVector3Normalize(cameraAxisX);

	//�J�����̎�(�����)
	DirectX::XMVECTOR cameraAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = DirectX::XMVector3Cross(cameraAxisZ, cameraAxisX);

	//�J������]�s��
	DirectX::XMMATRIX matCameraRot;
	//�J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	//�]�n�ɂ��t�s��(�t��])���v�Z
	DirectX::XMMATRIX matView = DirectX::XMMatrixTranspose(matCameraRot);

	//���_���W��-1���|�������W
	DirectX::XMVECTOR reverseEyePosition = DirectX::XMVectorNegate(eyePosition);
	//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��(�J�������W�n)
	DirectX::XMVECTOR tX = DirectX::XMVector3Dot(cameraAxisX, reverseEyePosition);	//X����
	DirectX::XMVECTOR tY = DirectX::XMVector3Dot(cameraAxisY, reverseEyePosition);	//Y����
	DirectX::XMVECTOR tZ = DirectX::XMVector3Dot(cameraAxisZ, reverseEyePosition);	//Z����
	//��̃x�N�g���ɂ܂Ƃ߂�
	DirectX::XMVECTOR translation = DirectX::XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//�r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;

	//Y�����r���{�[�h�s��̌v�Z
	//�J����X��, Y��, Z��
	DirectX::XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X���͋���
	ybillCameraAxisX = cameraAxisX;
	//Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = DirectX::XMVector3Normalize(upVector);
	//Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = DirectX::XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	//Y�����r���{�[�h�s��
	DirectX::XMMATRIX matBillboardY;
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	//���[���h�s��̍���
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(object->GetScale().x, object->GetScale().y, object->GetScale().z);
	DirectX::XMMATRIX rot = DirectX::XMMatrixIdentity();
	rot = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(object->GetRotation().z));
	rot = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(object->GetRotation().x));
	rot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(object->GetRotation().y));
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(object->GetPosition().x, object->GetPosition().y, object->GetPosition().z);
	world *= scale;
	world *= rot;
	world *= matBillboardY;
	world *= trans;

	//Y���r���{�[�h�s����Z�b�g
	object->SetMatWorld(world);
}

void SpeechBubble::Draw()
{
	object->Draw();
}
