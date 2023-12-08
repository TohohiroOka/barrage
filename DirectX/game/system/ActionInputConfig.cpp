#include "ActionInputConfig.h"
#include "WindowApp.h"

void (ActionInputConfig::* ActionInputConfig::phaseFuncTable[])() = {
	&ActionInputConfig::SelectModeUpdate,
	&ActionInputConfig::InputChangeModeUpdate,
};

DirectX::XMFLOAT4 ActionInputConfig::normalColor = { 1,1,1,1 };
DirectX::XMFLOAT4 ActionInputConfig::noChangeColor = { 0.5f, 0.5f, 0.5f, 1 };
DirectX::XMFLOAT4 ActionInputConfig::selectColor = { 1, 1, 0.5f, 1 };

void ActionInputConfig::LoadTexture()
{
	//仮置き背景用テクスチャ
	Sprite::LoadTexture("white1x1", "Resources/SubTexture/white1x1.png", false);

	//フレームテクスチャ読み込み
	Sprite::LoadTexture("inputFrame", "Resources/SpriteTexture/inputFrame.png", false);

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
	Sprite::LoadTexture("normal", "Resources/SpriteTexture/normal.png", false);
	Sprite::LoadTexture("reverse", "Resources/SpriteTexture/reverse.png", false);

	//操作方法用テキストテクスチャ読み込み
	Sprite::LoadTexture("select", "Resources/SpriteTexture/action/Select.png", false);
	Sprite::LoadTexture("back", "Resources/SpriteTexture/action/Back.png", false);
	Sprite::LoadTexture("buttonSelect", "Resources/SpriteTexture/action/ButtonSelect.png", false);
}

void ActionInputConfig::LoadActionNameTexture(int actionName, const std::string& fileName)
{
	std::string directoryPass = "Resources/SpriteTexture/action/";
	std::string keepname = "action_" + std::to_string(actionName);
	Sprite::LoadTexture(keepname, directoryPass + fileName, false);
}

void ActionInputConfig::LoadKeyTexture(BYTE num, const std::string& fileName)
{
	std::string directoryPass = "Resources/SpriteTexture/key/";
	std::string keepname = "key_" + std::to_string(num);
	Sprite::LoadTexture(keepname, directoryPass + fileName, false);
}

void ActionInputConfig::LoadPadTexture(int num, const std::string& fileName)
{
	std::string directoryPass = "Resources/SpriteTexture/pad/";
	std::string keepname = "pad_" + std::to_string(num);
	Sprite::LoadTexture(keepname, directoryPass + fileName, false);
}


ActionInputConfig::ActionInputConfig()
{
	//初期化
	Initialize();
}

ActionInputConfig::~ActionInputConfig()
{
}

void ActionInputConfig::Initialize()
{
	//仮置き背景スプライト初期化
	backSprite = Sprite::Create("white1x1", { 0,0 }, {}, { 0,0,0,0.7f });
	backSprite->SetSize({ (float)WindowApp::GetWindowWidth(), (float)WindowApp::GetWindowHeight() });

	//行動名スプライト初期化
	for (int i = 0; i < actionNameSprites.size(); i++) {
		const std::string name = "action_" + std::to_string(i);
		const DirectX::XMFLOAT2 pos = { 100, 100 + (float)i * 55 };
		actionNameSprites[i] = Sprite::Create(name, pos, { 0, 0.5f });
	}


	for (int i = Key; i < InputTypeNum; i++) {
		//各入力種類の要素数
		int loopNum = 0;
		if (i == CameraRota) { loopNum = CameraRotaTypeNum; }
		else { loopNum = GameInputManager::InputActionNum; }

		for (int j = 0; j < loopNum; j++) {
			//入力スプライト初期化
			std::string name;
			const DirectX::XMFLOAT2 leftTopPos = { 820, 100 };
			const DirectX::XMFLOAT2 posInterval = { 250, 55 };
			DirectX::XMFLOAT2 pos = { leftTopPos.x + (float)i * posInterval.x, leftTopPos.y + (float)j * posInterval.y };
			DirectX::XMFLOAT4 color = normalColor;
			if (i == Key) {
				name = "key_" + std::to_string(GameInputManager::GetKeyInputActionData(j).key);
				if (!GameInputManager::GetKeyInputActionData(j).isChangeInput) { color = noChangeColor; }
			}
			else if (i == PadButton) {
				name = "pad_" + std::to_string(GameInputManager::GetPadInputActionData(j).padButton);
				if (!GameInputManager::GetPadInputActionData(j).isChangeInput) { color = noChangeColor; }
			}
			else if (i == CameraRota) {
				if (j == CameraUpDown) {
					if (!GameInputManager::GetIsCameraRotaXReverse()) { name = "normal"; }
					else { name = "reverse"; }
					pos.y = leftTopPos.y + (float)GameInputManager::CameraUpRota * posInterval.y + posInterval.y / 2;
				}
				else if (j == CameraLeftRight) {
					if (!GameInputManager::GetIsCameraRotaYReverse()) { name = "normal"; }
					else { name = "reverse"; }
					pos.y = leftTopPos.y + (float)GameInputManager::CameraLeftRota * posInterval.y + posInterval.y / 2;
				}
			}
			std::unique_ptr<Sprite> newConfigSprite = Sprite::Create(name, pos, { 0.5f, 0.5f }, color);
			configSprites[i].push_back(std::move(newConfigSprite));

			//フレームスプライト初期化
			std::unique_ptr<Sprite> newFrameSprite = Sprite::Create("inputFrame", pos, { 0.5f, 0.5f }, color);
			frameSprites[i].push_back(std::move(newFrameSprite));
		}
	}

	//説明スプライト初期化
	infoSprites[InfoName::A_Button] = Sprite::Create("pad_" + std::to_string(XInputManager::PAD_A), { 1210, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Select] = Sprite::Create("select", { 1280, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::B_Button] = Sprite::Create("pad_" + std::to_string(XInputManager::PAD_B), { 1355, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Back] = Sprite::Create("back", { 1415, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::SelectButton] = Sprite::Create("buttonSelect", { 1305, 780 }, { 0.5f, 0.5f });

	//選択中のスプライトの枠の色を黄色くする
	frameSprites[selectInputType][selectAction]->SetColor(selectColor);
}

void ActionInputConfig::Update()
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

void ActionInputConfig::Draw()
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
		for (int i = 0; i <= InfoName::Back; i++) {
			infoSprites[i]->Draw();
		}
	}
	else {
		infoSprites[InfoName::SelectButton]->Draw();
	}
}

void ActionInputConfig::Reset()
{
	isInputConfigEnd = false;

	selectAction = 0;
	selectInputType = 0;

	//フレームスプライトの色を戻す
	for (int i = 0; i < frameSprites.size(); i++) {
		for (int j = 0; j < frameSprites[i].size(); j++) {
			DirectX::XMFLOAT4 color = normalColor;
			if (i == Key && !GameInputManager::GetKeyInputActionData(j).isChangeInput) { color = noChangeColor; }
			else if (i == PadButton && !GameInputManager::GetPadInputActionData(j).isChangeInput) { color = noChangeColor; }
			frameSprites[i][j]->SetColor(color);
		}
	}

	//選択中のスプライトの枠の色を黄色くする
	frameSprites[selectInputType][selectAction]->SetColor(selectColor);

	//フレームスプライト更新
	for (int i = 0; i < frameSprites.size(); i++) {
		for (int j = 0; j < frameSprites[i].size(); j++) {
			frameSprites[i][j]->Update();
		}
	}
}

void ActionInputConfig::SelectModeUpdate()
{
	//選択入力形態変更
	if (selectInputType < CameraRota && (DirectInput::GetInstance()->TriggerKey(DIK_RIGHT) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_RIGHT))) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Key && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == PadButton && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//次の入力種類へ
		selectInputType++;

		//カメラ回転変更の要素数は他と異なるので、横にある行動によって数値を調整
		if (selectInputType == CameraRota) {
			if (selectAction <= GameInputManager::CameraDownRota) { selectAction = CameraUpDown; }
			else { selectAction = CameraLeftRight; }
		}

		//新たに選択する枠の色を変更
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}
	else if (selectInputType > Key && (DirectInput::GetInstance()->TriggerKey(DIK_LEFT) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_LEFT))) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Key && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == PadButton && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//カメラ回転変更から戻る場合は要素数が他と異なるので、横にある行動によって数値を調整
		if (selectInputType == CameraRota) {
			if (selectAction == CameraUpDown) { selectAction = GameInputManager::CameraUpRota; }
			else if (selectAction == CameraLeftRight) { selectAction = GameInputManager::CameraLeftRota; }
		}

		//前の入力種類へ
		selectInputType--;

		//新たに選択する枠の色を変更
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}

	//選択行動変更
	if (selectAction > 0 && (DirectInput::GetInstance()->TriggerKey(DIK_UP) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP))) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Key && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == PadButton && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//前の行動へ
		selectAction--;

		//新たに選択する枠の色を変更
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}
	else if (selectAction < (int)configSprites[selectInputType].size() - 1 && (DirectInput::GetInstance()->TriggerKey(DIK_DOWN) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN))) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Key && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == PadButton && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//次の行動へ
		selectAction++;

		//新たに選択する枠の色を変更
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}

	//変更する選択を確定
	if (DirectInput::GetInstance()->TriggerKey(DIK_SPACE) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A)) {
		if (selectInputType == Key && GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) {
			configSprites[Key][selectAction]->DeleteTexture();

			phase = Phase::InputChangeMode;
		}
		else if (selectInputType == PadButton && GameInputManager::GetPadInputActionData(selectAction).isChangeInput) {
			configSprites[PadButton][selectAction]->DeleteTexture();

			phase = Phase::InputChangeMode;
		}
		else if (selectInputType == CameraRota) {
			phase = Phase::InputChangeMode;
		}
	}
	//入力設定終了
	else if (DirectInput::GetInstance()->TriggerKey(DIK_TAB) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_B)) {
		isInputConfigEnd = true;
	}
}

void ActionInputConfig::InputChangeModeUpdate()
{
	//キー
	if (selectInputType == Key) {
		//キー割り当て更新
		BYTE newKey;
		if (DirectInput::GetInstance()->GetTriggerKey(newKey)) {
			//他の行動と被っていなければ進む
			if (GameInputManager::ChangeInputKey((GameInputManager::InputAction)selectAction, newKey)) {
				//指定したキーのテクスチャがあるか
				if (!Sprite::GetIsTextureName("key_" + std::to_string(newKey))) {
					return;
				}
				//テクスチャ割り当て
				configSprites[Key][selectAction]->SetTexture("key_" + std::to_string(newKey));

				//選択に戻る
				phase = Phase::SelectMode;
			}
		}
	}
	//ボタン
	else if (selectInputType == PadButton) {
		//ボタン割り当て更新
		int newButton;
		if (XInputManager::GetInstance()->GetTriggerButton(newButton)) {
			//他の行動と被っていなければ進む
			if (GameInputManager::ChangeInputPadButton((GameInputManager::InputAction)selectAction, newButton)) {
				//指定したボタンのテクスチャがあるか
				if (!Sprite::GetIsTextureName("pad_" + std::to_string(newButton))) {
					return;
				}

				//テクスチャ割り当て
				configSprites[PadButton][selectAction]->SetTexture("pad_" + std::to_string(newButton));

				//選択に戻る
				phase = Phase::SelectMode;
			}
		}
	}
	//カメラ回転
	else if (selectInputType == CameraRota) {
		//選択したカメラ回転の回転方向を逆にする
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

		//選択に戻る
		phase = Phase::SelectMode;
	}
}
