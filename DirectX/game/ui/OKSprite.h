#pragma once
#include "Object/2d/Sprite.h"
#include "GameHelper.h"
#include "Math/Timer.h"

/// <summary>
/// OK�ƕ\������X�v���C�g
/// </summary>
class OKSprite
{
public: //�����o�֐�
	OKSprite();
	~OKSprite();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �`��J�n
	/// </summary>
	void DrawStart();

	//getter
	bool GetIsDraw() { return isDraw; }
	bool GetIsScaleChange() { return isScaleChange; }

	//setter
	void SetIsDraw(bool isDraw) { this->isDraw = isDraw; }


private: //�����o�֐�
	/// <summary>
	/// �X�P�[���ύX���o
	/// </summary>
	void ScaleChange();

private: //�����o�ϐ�
	//OK�X�v���C�g
	std::unique_ptr<Sprite> okSprite;
	//�X�v���C�g�̃X�P�[����
	float spriteScale;
	//�X�P�[���ύX���o����
	bool isScaleChange = false;
	//�^�C�}�[
	std::unique_ptr<Engine::Timer> scaleChangeTimer;
	//�`�悷�邩
	bool isDraw = false;
};