#include "CharActionInputSprite.h"
#include "Input/XInputManager.h"

CharActionInputSprite::CharActionInputSprite(GameInputManager::InputAction inputAction, const DirectX::XMFLOAT2& pos, float scale, bool isDraw)
	: CharSprite(L"", pos, scale, isDraw)
{
	//行動を保存しておく
	this->inputAction = inputAction;	
	this->scale = scale;

	//入力スプライト初期化
	std::string texName;
	if (XInputManager::GetInstance()->ControllerConnectCheck()) {
		texName = "pad_" + std::to_string(GameInputManager::GetPadInputActionData(inputAction).padButton);
	}
	else {
		texName = "key_" + std::to_string(GameInputManager::GetKeyInputActionData(inputAction).key);
	}
	//スプライト生成
	charSprite = Sprite::Create(texName, pos, { 0.5f, 0.5f });
	charSprite->SetScale(this->scale);

	//描画するかセット
	this->isDraw = isDraw;
}

CharActionInputSprite::~CharActionInputSprite()
{
}

void CharActionInputSprite::Update()
{
	//描画しない設定なら抜ける
	if (!isDraw) { return; }

	//入力スプライト更新
	std::string texName;
	if (XInputManager::GetInstance()->ControllerConnectCheck()) {
		texName = "pad_" + std::to_string(GameInputManager::GetPadInputActionData(inputAction).padButton);
	}
	else {
		texName = "key_" + std::to_string(GameInputManager::GetKeyInputActionData(inputAction).key);
	}
	//テクスチャ割り当て
	charSprite->SetTexture(texName);
	charSprite->SetScale(this->scale);
	charSprite->Update();
}
