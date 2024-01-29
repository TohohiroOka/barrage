#pragma once
#include "Object/2d/Sprite.h"

/// <summary>
/// �`���[�g���A���p�|�[�Y���
/// </summary>
class TutorialPause
{
public: //�����o�֐�
	TutorialPause();
	~TutorialPause();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �|�[�Y��ʊJ�n
	/// </summary>
	void PauseStart();

	/// <summary>
	/// �|�[�Y��ʏI��
	/// </summary>
	void PauseEnd();

	//getter
	bool GetIsPauseDraw() { return isPauseDraw; }


private: //�����o�ϐ�
	//���u���w�i�p���X�v���C�g
	std::unique_ptr<Sprite> backSprite;
	//�|�[�Y��ʕ\������
	bool isPauseDraw = false;
};