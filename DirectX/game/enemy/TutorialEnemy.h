#pragma once
#include "Object/3d/Object3d.h"

/// <summary>
/// �`���[�g���A���p�G
/// </summary>
class TutorialEnemy
{
public: //�����o�֐�
	TutorialEnemy(const DirectX::XMFLOAT3& position);
	~TutorialEnemy();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �_���[�W��H�炤
	/// </summary>
	void Damage();

	//getter
	Object3d* GetObject3d() { return object.get(); }
	bool GetIsDamage() { return isDamage; }

private: //�����o�ϐ�
	//�I�u�W�F�N�g3d
	std::unique_ptr<Object3d> object;
	std::unique_ptr<Model> model;

	//�_���[�W�t���O
	bool isDamage = false;
};