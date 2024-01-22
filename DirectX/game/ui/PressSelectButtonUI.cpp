#include "PressSelectButtonUI.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"

const DirectX::XMFLOAT2 PressSelectButtonUI::position = { 1300, 130 };

PressSelectButtonUI::PressSelectButtonUI()
{
	buttonSprite = std::make_unique<CharActionInputSprite>(GameInputManager::InputAction::Select, position, 1.5f, true);

	TextureManager::LoadTexture("into", "Resources/SpriteTexture/action/into.png");
	DirectX::XMFLOAT2 textPos = position;
	const float textScale = 2.0f;
	textPos.y -= (buttonSprite->GetSprite()->GetSize().y / 2 + buttonSprite->GetSprite()->GetSize().y * 0.1f) * textScale;
	textSprite = Sprite::Create("into", textPos, { 0.5, 0.5f });
	textSprite->SetScale(textScale);

}

PressSelectButtonUI::~PressSelectButtonUI()
{
}

void PressSelectButtonUI::Update()
{
	buttonSprite->Update();
	textSprite->Update();
}

void PressSelectButtonUI::Draw()
{
	buttonSprite->Draw();
	textSprite->Draw();
}
