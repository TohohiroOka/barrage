#include "ActionInputConfigBase.h"
#include "WindowApp.h"

void (ActionInputConfigBase::* ActionInputConfigBase::phaseFuncTable[])() = {
	&ActionInputConfigBase::SelectModeUpdate,
	&ActionInputConfigBase::InputChangeModeUpdate,
};

DirectX::XMFLOAT4 ActionInputConfigBase::normalColor = { 1,1,1,1 };
DirectX::XMFLOAT4 ActionInputConfigBase::noChangeColor = { 0.5f, 0.5f, 0.5f, 1 };
DirectX::XMFLOAT4 ActionInputConfigBase::selectColor = { 1, 1, 0.5f, 1 };

void ActionInputConfigBase::LoadTexture()
{
	//仮置き背景用テクスチャ
	TextureManager::LoadTexture("white1x1", "Resources/SubTexture/white1x1.png", false);

	//フレームテクスチャ読み込み
	TextureManager::LoadTexture("inputFrame", "Resources/SpriteTexture/inputFrame.png", false);

	//行動名テクスチャ読み込み
	LoadActionNameTexture(GameInputManager::MoveForward, "MoveForward.png");
	LoadActionNameTexture(GameInputManager::MoveBack, "MoveBack.png");
	LoadActionNameTexture(GameInputManager::MoveLeft, "MoveLeft.png");
	LoadActionNameTexture(GameInputManager::MoveRight, "MoveRight.png");
	LoadActionNameTexture(GameInputManager::Avoid_Blink_Dash, "Avoid_Blink_Dash.png");
	LoadActionNameTexture(GameInputManager::Jump, "Jump.png");
	LoadActionNameTexture(GameInputManager::LightAttack, "LightAttack.png");
	LoadActionNameTexture(GameInputManager::StrongAttack, "StrongAttack.png");
	LoadActionNameTexture(GameInputManager::CameraUpRota, "CameraUpRota.png");
	LoadActionNameTexture(GameInputManager::CameraDownRota, "CameraDownRota.png");
	LoadActionNameTexture(GameInputManager::CameraLeftRota, "CameraLeftRota.png");
	LoadActionNameTexture(GameInputManager::CameraRightRota, "CameraRightRota.png");
	LoadActionNameTexture(GameInputManager::Lockon, "Lockon.png");
	//キーテクスチャ読み込み
	LoadKeyTexture(DIK_ESCAPE, "ESCAPE.png");
	LoadKeyTexture(DIK_1, "1.png");
	LoadKeyTexture(DIK_2, "2.png");
	LoadKeyTexture(DIK_3, "3.png");
	LoadKeyTexture(DIK_4, "4.png");
	LoadKeyTexture(DIK_5, "5.png");
	LoadKeyTexture(DIK_6, "6.png");
	LoadKeyTexture(DIK_7, "7.png");
	LoadKeyTexture(DIK_8, "8.png");
	LoadKeyTexture(DIK_9, "9.png");
	LoadKeyTexture(DIK_0, "0.png");
	LoadKeyTexture(DIK_MINUS, "MINUS.png");
	LoadKeyTexture(DIK_BACK, "BACK.png");
	LoadKeyTexture(DIK_Q, "Q.png");
	LoadKeyTexture(DIK_W, "W.png");
	LoadKeyTexture(DIK_E, "E.png");
	LoadKeyTexture(DIK_R, "R.png");
	LoadKeyTexture(DIK_T, "T.png");
	LoadKeyTexture(DIK_Y, "Y.png");
	LoadKeyTexture(DIK_U, "U.png");
	LoadKeyTexture(DIK_I, "I.png");
	LoadKeyTexture(DIK_O, "O.png");
	LoadKeyTexture(DIK_P, "P.png");
	LoadKeyTexture(DIK_LBRACKET, "LBRACKET.png");
	LoadKeyTexture(DIK_RBRACKET, "RBRACKET.png");
	LoadKeyTexture(DIK_RETURN, "RETURN.png");
	LoadKeyTexture(DIK_LCONTROL, "LCONTROL.png");
	LoadKeyTexture(DIK_A, "A.png");
	LoadKeyTexture(DIK_S, "S.png");
	LoadKeyTexture(DIK_D, "D.png");
	LoadKeyTexture(DIK_F, "F.png");
	LoadKeyTexture(DIK_G, "G.png");
	LoadKeyTexture(DIK_H, "H.png");
	LoadKeyTexture(DIK_J, "J.png");
	LoadKeyTexture(DIK_K, "K.png");
	LoadKeyTexture(DIK_L, "L.png");
	LoadKeyTexture(DIK_SEMICOLON, "SEMICOLON.png");
	LoadKeyTexture(DIK_LSHIFT, "LSHIFT.png");
	LoadKeyTexture(DIK_BACKSLASH, "BACKSLASH.png");
	LoadKeyTexture(DIK_Z, "Z.png");
	LoadKeyTexture(DIK_X, "X.png");
	LoadKeyTexture(DIK_C, "C.png");
	LoadKeyTexture(DIK_V, "V.png");
	LoadKeyTexture(DIK_B, "B.png");
	LoadKeyTexture(DIK_N, "N.png");
	LoadKeyTexture(DIK_M, "M.png");
	LoadKeyTexture(DIK_COMMA, "COMMA.png");
	LoadKeyTexture(DIK_PERIOD, "PERIOD.png");
	LoadKeyTexture(DIK_SLASH, "SLASH.png");
	LoadKeyTexture(DIK_RSHIFT, "RSHIFT.png");
	LoadKeyTexture(DIK_SPACE, "SPACE.png");
	LoadKeyTexture(DIK_YEN, "YEN.png");
	LoadKeyTexture(DIK_AT, "AT.png");
	LoadKeyTexture(DIK_COLON, "COLON.png");
	LoadKeyTexture(DIK_RCONTROL, "RCONTROL.png");
	LoadKeyTexture(DIK_UP, "UP.png");
	LoadKeyTexture(DIK_LEFT, "LEFT.png");
	LoadKeyTexture(DIK_RIGHT, "RIGHT.png");
	LoadKeyTexture(DIK_DOWN, "DOWN.png");
	LoadKeyTexture(DIK_LALT, "LALT.png");
	LoadKeyTexture(DIK_RALT, "RALT.png");
	LoadKeyTexture(DIK_CIRCUMFLEX, "CIRCUMFLEX.png");


	//パッドボタンテクスチャ読み込み
	LoadPadTexture(XInputManager::PAD_A, "A.png");
	LoadPadTexture(XInputManager::PAD_B, "B.png");
	LoadPadTexture(XInputManager::PAD_Y, "Y.png");
	LoadPadTexture(XInputManager::PAD_X, "X.png");
	LoadPadTexture(XInputManager::PAD_LB, "LB.png");
	LoadPadTexture(XInputManager::PAD_RB, "RB.png");
	LoadPadTexture(XInputManager::PAD_LT, "LT.png");
	LoadPadTexture(XInputManager::PAD_RT, "RT.png");
	LoadPadTexture(XInputManager::PAD_START, "START.png");
	LoadPadTexture(XInputManager::PAD_BACK, "BACK.png");
	LoadPadTexture(XInputManager::PAD_UP, "UP.png");
	LoadPadTexture(XInputManager::PAD_DOWN, "DOWN.png");
	LoadPadTexture(XInputManager::PAD_LEFT, "LEFT.png");
	LoadPadTexture(XInputManager::PAD_RIGHT, "RIGHT.png");
	LoadPadTexture(XInputManager::PAD_LEFT_STICK_PUSH, "LEFT_STICK_PUSH.png");
	LoadPadTexture(XInputManager::PAD_RIGHT_STICK_PUSH, "RIGHT_STICK_PUSH.png");
	LoadPadTexture(16, "LSTICK.png");
	LoadPadTexture(17, "RSTICK.png");

	//カメラ回転用「ノーマル」「リバース」テクスチャ読み込み
	TextureManager::LoadTexture("normal", "Resources/SpriteTexture/normal.png", false);
	TextureManager::LoadTexture("reverse", "Resources/SpriteTexture/reverse.png", false);

	//操作方法用テキストテクスチャ読み込み
	TextureManager::LoadTexture("select", "Resources/SpriteTexture/action/Select.png", false);
	TextureManager::LoadTexture("back", "Resources/SpriteTexture/action/Back.png", false);
	TextureManager::LoadTexture("buttonSelect", "Resources/SpriteTexture/action/ButtonSelect.png", false);
}

void ActionInputConfigBase::LoadActionNameTexture(int actionName, const std::string& fileName)
{
	std::string directoryPass = "Resources/SpriteTexture/action/";
	std::string keepname = "action_" + std::to_string(actionName);
	TextureManager::LoadTexture(keepname, directoryPass + fileName, false);
}

void ActionInputConfigBase::LoadKeyTexture(BYTE num, const std::string& fileName)
{
	std::string directoryPass = "Resources/SpriteTexture/key/";
	std::string keepname = "key_" + std::to_string(num);
	TextureManager::LoadTexture(keepname, directoryPass + fileName, false);
}

void ActionInputConfigBase::LoadPadTexture(int num, const std::string& fileName)
{
	std::string directoryPass = "Resources/SpriteTexture/pad/";
	std::string keepname = "pad_" + std::to_string(num);
	TextureManager::LoadTexture(keepname, directoryPass + fileName, false);
}


ActionInputConfigBase::ActionInputConfigBase()
{
	//仮置き背景スプライト初期化
	backSprite = Sprite::Create("white1x1", { 0,0 }, {}, { 0,0,0,0.7f });
	backSprite->SetSize({ (float)WindowApp::GetWindowWidth(), (float)WindowApp::GetWindowHeight() });

	//行動名スプライト初期化
	for (int i = 0; i < actionNameSprites.size(); i++) {
		const std::string name = "action_" + std::to_string(i);
		const DirectX::XMFLOAT2 pos = { 150, 75 + (float)i * 55 };
		actionNameSprites[i] = Sprite::Create(name, pos, { 0, 0.5f });
	}
}

ActionInputConfigBase::~ActionInputConfigBase()
{
}

void ActionInputConfigBase::Update()
{
	//フェーズ挙動
	(this->*phaseFuncTable[static_cast<size_t>(phase)])();

	backSprite->Update();

	//行動名スプライト更新
	for (int i = 0; i < actionNameSprites.size(); i++) {
		actionNameSprites[i]->Update();
	}

	//フレームスプライト更新
	for (int i = 0; i < frameSprites.size(); i++) {
		for (int j = 0; j < frameSprites[i].size(); j++) {
			frameSprites[i][j]->Update();
		}
	}

	//入力スプライト更新
	for (int i = 0; i < configSprites.size(); i++) {
		for (int j = 0; j < configSprites[i].size(); j++) {
			configSprites[i][j]->Update();
		}
	}

	//説明スプライト更新
	for (int i = 0; i < infoSprites.size(); i++) {
		infoSprites[i]->Update();
	}
}

void ActionInputConfigBase::Draw()
{
	backSprite->Draw();

	//行動名スプライト描画
	for (int i = 0; i < actionNameSprites.size(); i++) {
		actionNameSprites[i]->Draw();
	}

	//フレームスプライト描画
	for (int i = 0; i < frameSprites.size(); i++) {
		for (int j = 0; j < frameSprites[i].size(); j++) {
			frameSprites[i][j]->Draw();
		}
	}

	//入力スプライト描画
	for (int i = 0; i < configSprites.size(); i++) {
		for (int j = 0; j < configSprites[i].size(); j++) {
			configSprites[i][j]->Draw();
		}
	}

	//説明スプライト描画
	if (phase == Phase::SelectMode || selectInputType == CameraRota) {
		for (int i = 0; i <= InfoName::Back_Text; i++) {
			infoSprites[i]->Draw();
		}
	}
	else {
		infoSprites[InfoName::Button_Choice_Text]->Draw();
	}
}

void ActionInputConfigBase::Reset()
{
	isInputConfigEnd = false;

	selectAction = 0;
	selectInputType = 0;

	//カメラ回転状態再確認
	if (!GameInputManager::GetIsCameraRotaXReverse()) { configSprites[CameraRota][CameraUpDown]->SetTexture("normal"); }
	else { configSprites[CameraRota][CameraUpDown]->SetTexture("reverse"); }
	if (!GameInputManager::GetIsCameraRotaYReverse()) { configSprites[CameraRota][CameraLeftRight]->SetTexture("normal"); }
	else { configSprites[CameraRota][CameraLeftRight]->SetTexture("reverse"); }
}

void ActionInputConfigBase::CameraRotationChange()
{
	if (selectAction == CameraUpDown) {
		GameInputManager::SetIsCameraRotaXReverse(!(GameInputManager::GetIsCameraRotaXReverse()));

		//テクスチャ変更
		if (!GameInputManager::GetIsCameraRotaXReverse()) { configSprites[CameraRota][selectAction]->SetTexture("normal"); }
		else { configSprites[CameraRota][selectAction]->SetTexture("reverse"); }
	}
	else if (selectAction == CameraLeftRight) {
		GameInputManager::SetIsCameraRotaYReverse(!(GameInputManager::GetIsCameraRotaYReverse()));

		//テクスチャ変更
		if (!GameInputManager::GetIsCameraRotaYReverse()) { configSprites[CameraRota][selectAction]->SetTexture("normal"); }
		else { configSprites[CameraRota][selectAction]->SetTexture("reverse"); }
	}
}
