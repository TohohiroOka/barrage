#pragma once
#include "PlayerActionBase.h"
#include "Math/Timer.h"

/// <summary>
/// �v���C���[�u�����N�s��
/// </summary>
class PlayerActionBlink : public PlayerActionBase
{
public: //�����o�֐�
	PlayerActionBlink(Player* player);
	~PlayerActionBlink();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	///	���
	/// </summary>
	void Blink();

private: //�ÓI�����o�ϐ�
	//�u�����N�ɂ����鎞��
	static const int blinkTime = 20;
	//��s���͂��J�n���鎞��
	static const int actionChangeStartTime = 15;

private: //�����o�ϐ�
	//�u�����N�p�^�C�}�[
	std::unique_ptr<Engine::Timer> blinkTimer;
	//�u�����N�J�n�\��
	bool isBlinkStart = true;
};