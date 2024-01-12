#include "PressSelectButtonUI.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"

const DirectX::XMFLOAT2 PressSelectButtonUI::position = { 1300, 130 };

PressSelectButtonUI::PressSelectButtonUI()
{
	std::string name;
	if (XInputManager::GetInstance()->ControllerConnectCheck()) { name = "pad_" + std::to_string(XInputManager::PAD_A); }
	else { name = "key_" + std::to_string(DIK_SPACE); }
	buttonSprite = Sprite::Create(name, {}, { 0.5, 0.5f });
	buttonSprite->SetScale(2.0f);
	buttonSprite->SetPosition(position);

	TextureManager::LoadTexture("into", "Resources/SpriteTexture/action/into.png");
	DirectX::XMFLOAT2 textPos = position;
	textPos.y -= buttonSprite->GetSize().y - buttonSprite->GetSize().y * 0.1f;
	textSprite = Sprite::Create("into", textPos, { 0.5, 0.5f });
	textSprite->SetScale(2.0f);

}

PressSelectButtonUI::~PressSelectButtonUI()
{
}

void PressSelectButtonUI::Update()
{
	std::string name;
	if (XInputManager::GetInstance()->ControllerConnectCheck()) { name = "pad_" + std::to_string(XInputManager::PAD_A); }
	else { name = "key_" + std::to_string(DIK_SPACE); }
	buttonSprite->SetTexture(name);
	buttonSprite->SetScale(1.5f);

	buttonSprite->Update();
	textSprite->Update();
}

void PressSelectButtonUI::Draw()
{
	buttonSprite->Draw();
	textSprite->Draw();
}
