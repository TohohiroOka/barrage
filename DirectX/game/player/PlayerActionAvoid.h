#pragma once
#include "PlayerActionBase.h"
#include "Math/Timer.h"

/// <summary>
/// �v���C���[����s��
/// </summary>
class PlayerActionAvoid : public PlayerActionBase
{
public: //�����o�֐�
	PlayerActionAvoid(Player* player);
	~PlayerActionAvoid();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	///	���
	/// </summary>
	void Avoid();

private: //�ÓI�����o�ϐ�
	//����ɂ����鎞��
	static const int avoidTime = 42;
	//��s���͂��J�n���鎞��
	static const int actionChangeStartTime = 15;

private: //�����o�ϐ�
	//���p�^�C�}�[
	std::unique_ptr<Engine::Timer> avoidTimer;
};