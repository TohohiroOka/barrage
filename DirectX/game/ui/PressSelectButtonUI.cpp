#include "PressSelectButtonUI.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"

const DirectX::XMFLOAT2 PressSelectButtonUI::position = { 1300, 80 };

PressSelectButtonUI::PressSelectButtonUI()
{
	TextureManager::LoadTexture("into", "Resources/SpriteTexture/action/into.png");
	textSprite = Sprite::Create("into", position, { 0, 0.5f });
	textSprite->SetScale(2.0f);

	std::string name;
	if (XInputManager::GetInstance()->ControllerConnectCheck()) { name = "pad_" + std::to_string(XInputManager::PAD_A); }
	else { name = "key_" + std::to_string(DIK_E); }
	buttonSprite = Sprite::Create(name, {}, { 0, 0.5f });
	buttonSprite->SetScale(1.5f);
	DirectX::XMFLOAT2 buttonPos = position;
	buttonPos.x -= buttonSprite->GetSize().x;
	buttonSprite->SetPosition(buttonPos);
}

PressSelectButtonUI::~PressSelectButtonUI()
{
}

void PressSelectButtonUI::Update()
{
	std::string name;
	if (XInputManager::GetInstance()->ControllerConnectCheck()) { name = "pad_" + std::to_string(XInputManager::PAD_A); }
	else { name = "key_" + std::to_string(DIK_E); }
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
