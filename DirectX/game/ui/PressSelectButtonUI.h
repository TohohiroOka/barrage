#pragma once
#include "ui/CharActionInputSprite.h"

/// <summary>
/// �I���{�^����������󋵂�������UI(�uA:����v�Ȃ�)
/// </summary>
class PressSelectButtonUI
{
public: //�����o�֐�
	PressSelectButtonUI();
	~PressSelectButtonUI();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private: //�ÓI�����o�ϐ�
	//���W
	static const DirectX::XMFLOAT2 position;

private: //�����o�ϐ�
	std::unique_ptr<CharActionInputSprite> buttonSprite;
	std::unique_ptr<Sprite> textSprite;
};