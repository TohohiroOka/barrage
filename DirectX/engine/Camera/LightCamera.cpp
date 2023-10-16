#include "LightCamera.h"
#include "WindowApp.h"

LightCamera::LightCamera(const Vector3& eye, const Vector3& target):
	Camera(false)
{
	//���_�A�����_���Z�b�g
	this->eye = eye;
	this->target = target;
	up = { 0, 1, 0 };

	UpdateMatProjection();
}

LightCamera::~LightCamera()
{
}

void LightCamera::Update()
{
	if (dirtyProjection) {
		//�������e�ɂ��ˉe�s��̐���
		UpdateMatProjection();
		//�t���O��������
		dirtyProjection = false;
	}

	Camera::Update();
}

void LightCamera::UpdateMatProjection()
{
	matProjection = DirectX::XMMatrixOrthographicOffCenterLH(
		projectionMin.x, projectionMax.x,
		projectionMin.y, projectionMax.y,
		0.1f, 2000.0f
	);
}
