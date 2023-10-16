#pragma once
#include "Camera.h"
#include "engine/Math/Vector2.h"

class LightCamera : public Camera
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	LightCamera(const Vector3& eye, const Vector3& target = { 0, 0, 0 });

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~LightCamera() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	void UpdateMatProjection();

	//setter
	void SetEyeTarget(const Vector3& eye, const Vector3& target) {
		this->eye = eye;
		this->target = target;
	}

	void SetProjectionNum(const Vector2& projectionMax, const Vector2& projectionMin) {
		this->projectionMax = projectionMax;
		this->projectionMin = projectionMin;
		dirtyProjection = true;
	}

private: //�����o�ϐ�
	//�v���W�F�N�V�����̍ő�l
	Vector2 projectionMax = { 100, 100 };
	//�v���W�F�N�V�����̍ŏ��l
	Vector2 projectionMin = { -100, -100 };
	//�_�[�e�B�t���O
	bool dirtyProjection = false;
};