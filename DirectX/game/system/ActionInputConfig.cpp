#include "ActionInputConfig.h"
#include "WindowApp.h"

void (ActionInputConfig::* ActionInputConfig::phaseFuncTable[])() = {
	&ActionInputConfig::Select,
	&ActionInputConfig::InputChange,
};

DirectX::XMFLOAT4 ActionInputConfig::normalColor = { 1,1,1,1 };
DirectX::XMFLOAT4 ActionInputConfig::noChangeColor = { 0.5f, 0.5f, 0.5f, 1 };
DirectX::XMFLOAT4 ActionInputConfig::selectColor = { 1, 1, 0.5f, 1 };

void ActionInputConfig::LoadTexture()
{
	//仮置き背景用テクスチャ
	Sprite::LoadTexture("white1x1", "Resources/SubTexture/white1x1.png");

	//フレームテクスチャ読み込み
	Sprite::LoadTexture("inputFrame", "Resources/SpriteTexture/inputFrame.png");

	//行動名テクスチャ読み込み
	LoadActionNameTexture(GameInputManager::MoveForward, "MoveForward.png");
	LoadActionNameTexture(GameInputManager::MoveBack, "MoveBack.png");
	LoadActionNameTexture(GameInputManager::MoveLeft, "MoveLeft.png");
	LoadActionNameTexture(GameInputManager::MoveRight, "MoveRight.png");
	LoadActionNameTexture(GameInputManager::Avoid_Blink_Dash, "Avoid_Blink_Dash.png");
	LoadActionNameTexture(GameInputManager::Jump, "Jump.png");
	LoadActionNameTexture(GameInputManager::Attack, "Attack.png");
	LoadActionNameTexture(GameInputManager::MoveCameraUp, "MoveCameraUp.png");
	LoadActionNameTexture(GameInputManager::MoveCameraDown, "MoveCameraDown.png");
	LoadActionNameTexture(GameInputManager::MoveCameraLeft, "MoveCameraLeft.png");
	LoadActionNameTexture(GameInputManager::MoveCameraRight, "MoveCameraRight.png");
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
}

void ActionInputConfig::LoadActionNameTexture(int actionName, const std::string& fileName)
{
	std::string directoryPass = "Resources/SpriteTexture/action/";
	std::string keepname = "action_" + std::to_string(actionName);
	Sprite::LoadTexture(keepname, directoryPass + fileName);
}

void ActionInputConfig::LoadKeyTexture(BYTE num, const std::string& fileName)
{
	std::string directoryPass = "Resources/SpriteTexture/key/";
	std::string keepname = "key_" + std::to_string(num);
	Sprite::LoadTexture(keepname, directoryPass + fileName);
}

void ActionInputConfig::LoadPadTexture(int num, const std::string& fileName)
{
	std::string directoryPass = "Resources/SpriteTexture/pad/";
	std::string keepname = "pad_" + std::to_string(num);
	Sprite::LoadTexture(keepname, directoryPass + fileName);
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
	backSprite = std::make_unique<Sprite>();
	backSprite->Initialize("white1x1", { 0,0 }, {}, { 0,0,0,0.7f });
	backSprite->SetSize({ (float)WindowApp::GetWindowWidth(), (float)WindowApp::GetWindowHeight() });

	//行動名スプライト初期化
	for (int i = 0; i < actionNameSprites.size(); i++) {
		actionNameSprites[i] = std::make_unique<Sprite>();
		const std::string name = "action_" + std::to_string(i);
		const DirectX::XMFLOAT2 pos = { 100, 100 + (float)i * 55 };
		actionNameSprites[i]->Initialize(name, pos, { 0, 0.5f });
	}

	//入力スプライト初期化
	for (int i = 0; i < configSprites[0].size(); i++) {
		configSprites[0][i] = std::make_unique<Sprite>();
		const std::string name = "key_" + std::to_string(GameInputManager::GetKeyInputActionData(i).key);
		const DirectX::XMFLOAT2 pos = { 820, 100 + (float)i * 55 };
		DirectX::XMFLOAT4 color = normalColor;
		if (!GameInputManager::GetKeyInputActionData(i).isChangeInput) { color = noChangeColor; }
		configSprites[0][i]->Initialize(name, pos, { 0.5f, 0.5f }, color);

		//フレームスプライト初期化
		frameSprites[0][i] = std::make_unique<Sprite>();
		frameSprites[0][i]->Initialize("inputFrame", pos, { 0.5f, 0.5f }, color);
	}
	for (int i = 0; i < configSprites[1].size(); i++) {
		configSprites[1][i] = std::make_unique<Sprite>();
		const std::string name = "pad_" + std::to_string(GameInputManager::GetPadInputActionData(i).padButton);
		const DirectX::XMFLOAT2 pos = { 1070, 100 + (float)i * 55 };
		DirectX::XMFLOAT4 color = normalColor;
		if (!GameInputManager::GetPadInputActionData(i).isChangeInput) { color = noChangeColor; }
		configSprites[1][i]->Initialize(name, pos, { 0.5f, 0.5f }, color);

		//フレームスプライト初期化
		frameSprites[1][i] = std::make_unique<Sprite>();
		frameSprites[1][i]->Initialize("inputFrame", pos, { 0.5f, 0.5f }, color);
	}

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
			if (i == 0 && !GameInputManager::GetKeyInputActionData(j).isChangeInput) { color = noChangeColor; }
			else if (i == 1 && !GameInputManager::GetPadInputActionData(j).isChangeInput) { color = noChangeColor; }
			frameSprites[i][j]->SetColor(color);
		}
	}

	//選択中のスプライトの枠の色を黄色くする
	frameSprites[selectInputType][selectAction]->SetColor(selectColor);
}

void ActionInputConfig::Select()
{
	//選択入力形態変更
	if (DirectInput::GetInstance()->TriggerKey(DIK_RIGHT) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_RIGHT)) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == 0 && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == 1 && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		selectInputType++;
		selectInputType = min(selectInputType, 1);

		//新たに選択する枠の色を変更
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}
	else if (DirectInput::GetInstance()->TriggerKey(DIK_LEFT) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_LEFT)) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == 0 && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == 1 && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		selectInputType--;
		selectInputType = max(selectInputType, 0);

		//新たに選択する枠の色を変更
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}

	//選択行動変更
	if (DirectInput::GetInstance()->TriggerKey(DIK_UP) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP)) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == 0 && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == 1 && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		selectAction--;
		selectAction = max(selectAction, 0);

		//新たに選択する枠の色を変更
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}
	else if (DirectInput::GetInstance()->TriggerKey(DIK_DOWN) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN)) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == 0 && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == 1 && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		selectAction++;
		selectAction = min(selectAction, GameInputManager::InputActionNum - 1);

		//新たに選択する枠の色を変更
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}

	//変更する選択を確定
	if (DirectInput::GetInstance()->TriggerKey(DIK_SPACE) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A)) {
		if (selectInputType == 0 && GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) {
			configSprites[0][selectAction]->DeleteTexture();

			phase = Phase::InputChangeMode;
		}
		else if (selectInputType == 1 && GameInputManager::GetPadInputActionData(selectAction).isChangeInput) {
			configSprites[1][selectAction]->DeleteTexture();

			phase = Phase::InputChangeMode;
		}
	}
	//入力設定終了
	else if (DirectInput::GetInstance()->TriggerKey(DIK_F1) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_B)) {
		isInputConfigEnd = true;
	}
}

void ActionInputConfig::InputChange()
{
	//キーorボタン変更の場合
	if (selectAction <= GameInputManager::InputActionNum - 1) {
		//キー
		if (selectInputType == 0) {
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
					configSprites[0][selectAction]->SetTexture("key_" + std::to_string(newKey));

					//選択に戻る
					phase = Phase::SelectMode;
				}
			}
		}
		//ボタン
		else if (selectInputType == 1) {
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
					configSprites[1][selectAction]->SetTexture("pad_" + std::to_string(newButton));

					//選択に戻る
					phase = Phase::SelectMode;
				}
			}
		}
	}
}
