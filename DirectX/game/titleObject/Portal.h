#pragma once
#include "Object/3d/Object3d.h"
#include "Math/Vector3.h"
#include "player/PlayerData.h"

class InterfaceScene;

/// <summary>
/// �V�[�����m���q���|�[�^��
/// </summary>
class Portal
{
public: //�����o�֐�
	Portal(const Vector3& position, InterfaceScene* changeScene);
	~Portal();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	/// <param name="playerRota">�v���C���[�̉�]�p</param>
	/// <param name="isPlayerOnGround">�v���C���[���n�ʂɐڒn���Ă��邩</param>
	void Update(const PlayerData& playerData);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e�p�����J�������_�`��
	/// </summary>
	void DrawLightView();

	//getter
	Object3d* GetObject3d() { return object.get(); }
	const Vector3& GetIntoPortalRangeMin() { return intoPortalRangeMin; }
	const Vector3& GetIntoPortalRangeMax() { return intoPortalRangeMax; }
	bool GetIsIntoPortal() { return isIntoPortal; }
	InterfaceScene* GetChangeScene() { return changeScene; }

private: //�����o�֐�
	/// <summary>
	/// �|�[�^���ɓ����͈͂ɂ��邩�`�F�b�N
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	/// <param name="playerRota">�v���C���[�̉�]�p</param>
	/// <returns>�|�[�^���ɓ����͈͂ɂ��邩</returns>
	bool CheckIntoPortal(const PlayerData& playerData);

private: //�����o�ϐ�
	//�I�u�W�F�N�g3d
	std::unique_ptr<Object3d> object;
	std::unique_ptr<Model> model;
	//�|�[�^���ɓ����͈͍ŏ��l�ƍő�l
	Vector3 intoPortalRangeMin;
	Vector3 intoPortalRangeMax;
	//�|�[�^���ɓ���邩
	bool isIntoPortal = false;
	//�ύX��̃V�[��
	InterfaceScene* changeScene;
};