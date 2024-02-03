#include "ActionInputConfigKey.h"

ActionInputConfigKey::ActionInputConfigKey()
	: ActionInputConfigBase()
{
	for (int i = Config; i < InputTypeNum; i++) {
		//各入力種類の要素数
		int loopNum = 0;
		if (i == CameraRota) { loopNum = CameraRotaTypeNum; }
		else { loopNum = GameInputManager::Lockon + 1; }

		for (int j = 0; j < loopNum; j++) {
			//入力スプライト初期化
			std::string name;
			const DirectX::XMFLOAT2 leftTopPos = { 940, 75 };
			const DirectX::XMFLOAT2 posInterval = { 250, 55 };
			DirectX::XMFLOAT2 pos = { leftTopPos.x + (float)i * posInterval.x, leftTopPos.y + (float)j * posInterval.y };
			DirectX::XMFLOAT4 color = normalColor;
			if (i == Config) {
				name = "key_" + std::to_string(GameInputManager::GetKeyInputActionData(j).key);
				if (!GameInputManager::GetKeyInputActionData(j).isChangeInput) { color = noChangeColor; }
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

	//選択中のスプライトの枠の色を黄色くする
	frameSprites[selectInputType][selectAction]->SetColor(selectColor);

	//説明スプライト初期化
	infoSprites[InfoName::Select_Button] = Sprite::Create("key_" + std::to_string(GameInputManager::GetKeyInputActionData(GameInputManager::Select).key), { 1210, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Select_Text] = Sprite::Create("select", { 1280, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Back_Button] = Sprite::Create("key_" + std::to_string(GameInputManager::GetKeyInputActionData(GameInputManager::Back).key), { 1355, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Back_Text] = Sprite::Create("back", { 1415, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Button_Choice_Text] = Sprite::Create("buttonSelect", { 1305, 780 }, { 0.5f, 0.5f });
}

ActionInputConfigKey::~ActionInputConfigKey()
{
}

void ActionInputConfigKey::Reset()
{
	ActionInputConfigBase::Reset();

	//フレームスプライトの色を戻す
	for (int i = 0; i < frameSprites.size(); i++) {
		for (int j = 0; j < frameSprites[i].size(); j++) {
			DirectX::XMFLOAT4 color = normalColor;
			if (i == Config && !GameInputManager::GetKeyInputActionData(j).isChangeInput) { color = noChangeColor; }
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

void ActionInputConfigKey::SelectModeUpdate()
{
	//選択入力形態変更
	if (selectInputType < CameraRota && (DirectInput::GetInstance()->TriggerKey(DIK_D) || DirectInput::GetInstance()->TriggerKey(DIK_RIGHT) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_RIGHT) || XInputManager::GetInstance()->TriggerLeftStickX(false))) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Config && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
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
	else if (selectInputType > Config && (DirectInput::GetInstance()->TriggerKey(DIK_A) || DirectInput::GetInstance()->TriggerKey(DIK_LEFT) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_LEFT) || XInputManager::GetInstance()->TriggerLeftStickX(true))) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Config && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
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
	if (selectAction > 0 && (DirectInput::GetInstance()->TriggerKey(DIK_W) || DirectInput::GetInstance()->TriggerKey(DIK_UP) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP) || XInputManager::GetInstance()->TriggerLeftStickY(true))) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Config && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//前の行動へ
		selectAction--;

		//新たに選択する枠の色を変更
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}
	else if (selectAction < (int)configSprites[selectInputType].size() - 1 && (DirectInput::GetInstance()->TriggerKey(DIK_S) || DirectInput::GetInstance()->TriggerKey(DIK_DOWN) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN) || XInputManager::GetInstance()->TriggerLeftStickY(false))) {
		//選択中のスプライトの枠の色を元に戻す
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Config && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//次の行動へ
		selectAction++;

		//新たに選択する枠の色を変更
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}

	//変更する選択を確定
	if (DirectInput::GetInstance()->TriggerKey(DIK_SPACE)) {
		if (selectInputType == Config && GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) {
			configSprites[Config][selectAction]->DeleteTexture();

			phase = Phase::InputChangeMode;
		}
		else if (selectInputType == CameraRota) {
			phase = Phase::InputChangeMode;
		}
	}
	//入力設定終了
	else if (GameInputManager::TriggerInputAction(GameInputManager::Back) || GameInputManager::TriggerInputAction(GameInputManager::Pause)) {
		isInputConfigEnd = true;
	}
}

void ActionInputConfigKey::InputChangeModeUpdate()
{
	//キー
	if (selectInputType == Config) {
		//キー割り当て更新
		BYTE newKey;
		if (DirectInput::GetInstance()->GetTriggerKey(newKey)) {
			//他の行動と被っていなければ進む
			if (GameInputManager::ChangeInputKey((GameInputManager::InputAction)selectAction, newKey)) {
				//指定したキーのテクスチャがあるか
				if (!TextureManager::GetIsTextureName("key_" + std::to_string(newKey))) {
					return;
				}
				//テクスチャ割り当て
				configSprites[Config][selectAction]->SetTexture("key_" + std::to_string(newKey));

				//選択に戻る
				phase = Phase::SelectMode;
			}
		}
	}
	//カメラ回転
	else if (selectInputType == CameraRota) {
		//選択したカメラ回転の回転方向を逆にする
		CameraRotationChange();

		//選択に戻る
		phase = Phase::SelectMode;
	}
}
