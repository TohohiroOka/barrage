#pragma once
#include "CharSprite.h"
#include "system/GameInputManager.h"

/// <summary>
/// 1�����Ƃ��Ĉ����s�����̓X�v���C�g
/// </summary>
class CharActionInputSprite : public CharSprite
{
public: //�����o�֐�
	CharActionInputSprite(GameInputManager::InputAction inputAction, const DirectX::XMFLOAT2& pos, float scale, bool isDraw = true);
	~CharActionInputSprite();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�ϐ�
	//�s����
	GameInputManager::InputAction inputAction;
	//�傫��
	float scale;
};