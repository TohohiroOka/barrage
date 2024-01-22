#include "CharActionInputSprite.h"
#include "Input/XInputManager.h"

CharActionInputSprite::CharActionInputSprite(GameInputManager::InputAction inputAction, const DirectX::XMFLOAT2& pos, float scale, bool isDraw)
	: CharSprite(L"", pos, scale, isDraw)
{
	//�s����ۑ����Ă���
	this->inputAction = inputAction;	
	this->scale = scale;

	//���̓X�v���C�g������
	std::string texName;
	if (XInputManager::GetInstance()->ControllerConnectCheck()) {
		texName = "pad_" + std::to_string(GameInputManager::GetPadInputActionData(inputAction).padButton);
	}
	else {
		texName = "key_" + std::to_string(GameInputManager::GetKeyInputActionData(inputAction).key);
	}
	//�X�v���C�g����
	charSprite = Sprite::Create(texName, pos, { 0.5f, 0.5f });
	charSprite->SetScale(this->scale);

	//�`�悷�邩�Z�b�g
	this->isDraw = isDraw;
}

CharActionInputSprite::~CharActionInputSprite()
{
}

void CharActionInputSprite::Update()
{
	//�`�悵�Ȃ��ݒ�Ȃ甲����
	if (!isDraw) { return; }

	//���̓X�v���C�g�X�V
	std::string texName;
	if (XInputManager::GetInstance()->ControllerConnectCheck()) {
		texName = "pad_" + std::to_string(GameInputManager::GetPadInputActionData(inputAction).padButton);
	}
	else {
		texName = "key_" + std::to_string(GameInputManager::GetKeyInputActionData(inputAction).key);
	}
	//�e�N�X�`�����蓖��
	charSprite->SetTexture(texName);
	charSprite->SetScale(this->scale);
	charSprite->Update();
}
