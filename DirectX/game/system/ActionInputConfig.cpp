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
	//���u���w�i�p�e�N�X�`��
	Sprite::LoadTexture("white1x1", "Resources/SubTexture/white1x1.png", false);

	//�t���[���e�N�X�`���ǂݍ���
	Sprite::LoadTexture("inputFrame", "Resources/SpriteTexture/inputFrame.png", false);

	//�s�����e�N�X�`���ǂݍ���
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

	//�L�[�e�N�X�`���ǂݍ���
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


	//�p�b�h�{�^���e�N�X�`���ǂݍ���
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

	//�J������]�p�u�m�[�}���v�u���o�[�X�v�e�N�X�`���ǂݍ���
	Sprite::LoadTexture("normal", "Resources/SpriteTexture/normal.png", false);
	Sprite::LoadTexture("reverse", "Resources/SpriteTexture/reverse.png", false);

	//������@�p�e�L�X�g�e�N�X�`���ǂݍ���
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
	//������
	Initialize();
}

ActionInputConfig::~ActionInputConfig()
{
}

void ActionInputConfig::Initialize()
{
	//���u���w�i�X�v���C�g������
	backSprite = Sprite::Create("white1x1", { 0,0 }, {}, { 0,0,0,0.7f });
	backSprite->SetSize({ (float)WindowApp::GetWindowWidth(), (float)WindowApp::GetWindowHeight() });

	//�s�����X�v���C�g������
	for (int i = 0; i < actionNameSprites.size(); i++) {
		const std::string name = "action_" + std::to_string(i);
		const DirectX::XMFLOAT2 pos = { 100, 100 + (float)i * 55 };
		actionNameSprites[i] = Sprite::Create(name, pos, { 0, 0.5f });
	}


	for (int i = Key; i < InputTypeNum; i++) {
		//�e���͎�ނ̗v�f��
		int loopNum = 0;
		if (i == CameraRota) { loopNum = CameraRotaTypeNum; }
		else { loopNum = GameInputManager::InputActionNum; }

		for (int j = 0; j < loopNum; j++) {
			//���̓X�v���C�g������
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

			//�t���[���X�v���C�g������
			std::unique_ptr<Sprite> newFrameSprite = Sprite::Create("inputFrame", pos, { 0.5f, 0.5f }, color);
			frameSprites[i].push_back(std::move(newFrameSprite));
		}
	}

	//�����X�v���C�g������
	infoSprites[InfoName::A_Button] = Sprite::Create("pad_" + std::to_string(XInputManager::PAD_A), { 1210, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Select] = Sprite::Create("select", { 1280, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::B_Button] = Sprite::Create("pad_" + std::to_string(XInputManager::PAD_B), { 1355, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Back] = Sprite::Create("back", { 1415, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::SelectButton] = Sprite::Create("buttonSelect", { 1305, 780 }, { 0.5f, 0.5f });

	//�I�𒆂̃X�v���C�g�̘g�̐F�����F������
	frameSprites[selectInputType][selectAction]->SetColor(selectColor);
}

void ActionInputConfig::Update()
{
	//�t�F�[�Y����
	(this->*phaseFuncTable[static_cast<size_t>(phase)])();

	backSprite->Update();

	//�s�����X�v���C�g�X�V
	for (int i = 0; i < actionNameSprites.size(); i++) {
		actionNameSprites[i]->Update();
	}

	//�t���[���X�v���C�g�X�V
	for (int i = 0; i < frameSprites.size(); i++) {
		for (int j = 0; j < frameSprites[i].size(); j++) {
			frameSprites[i][j]->Update();
		}
	}

	//���̓X�v���C�g�X�V
	for (int i = 0; i < configSprites.size(); i++) {
		for (int j = 0; j < configSprites[i].size(); j++) {
			configSprites[i][j]->Update();
		}
	}

	//�����X�v���C�g�X�V
	for (int i = 0; i < infoSprites.size(); i++) {
		infoSprites[i]->Update();
	}
}

void ActionInputConfig::Draw()
{
	backSprite->Draw();

	//�s�����X�v���C�g�`��
	for (int i = 0; i < actionNameSprites.size(); i++) {
		actionNameSprites[i]->Draw();
	}

	//�t���[���X�v���C�g�`��
	for (int i = 0; i < frameSprites.size(); i++) {
		for (int j = 0; j < frameSprites[i].size(); j++) {
			frameSprites[i][j]->Draw();
		}
	}

	//���̓X�v���C�g�`��
	for (int i = 0; i < configSprites.size(); i++) {
		for (int j = 0; j < configSprites[i].size(); j++) {
			configSprites[i][j]->Draw();
		}
	}

	//�����X�v���C�g�`��
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

	//�t���[���X�v���C�g�̐F��߂�
	for (int i = 0; i < frameSprites.size(); i++) {
		for (int j = 0; j < frameSprites[i].size(); j++) {
			DirectX::XMFLOAT4 color = normalColor;
			if (i == Key && !GameInputManager::GetKeyInputActionData(j).isChangeInput) { color = noChangeColor; }
			else if (i == PadButton && !GameInputManager::GetPadInputActionData(j).isChangeInput) { color = noChangeColor; }
			frameSprites[i][j]->SetColor(color);
		}
	}

	//�I�𒆂̃X�v���C�g�̘g�̐F�����F������
	frameSprites[selectInputType][selectAction]->SetColor(selectColor);

	//�t���[���X�v���C�g�X�V
	for (int i = 0; i < frameSprites.size(); i++) {
		for (int j = 0; j < frameSprites[i].size(); j++) {
			frameSprites[i][j]->Update();
		}
	}
}

void ActionInputConfig::SelectModeUpdate()
{
	//�I����͌`�ԕύX
	if (selectInputType < CameraRota && (DirectInput::GetInstance()->TriggerKey(DIK_RIGHT) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_RIGHT))) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Key && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == PadButton && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//���̓��͎�ނ�
		selectInputType++;

		//�J������]�ύX�̗v�f���͑��ƈقȂ�̂ŁA���ɂ���s���ɂ���Đ��l�𒲐�
		if (selectInputType == CameraRota) {
			if (selectAction <= GameInputManager::CameraDownRota) { selectAction = CameraUpDown; }
			else { selectAction = CameraLeftRight; }
		}

		//�V���ɑI������g�̐F��ύX
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}
	else if (selectInputType > Key && (DirectInput::GetInstance()->TriggerKey(DIK_LEFT) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_LEFT))) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Key && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == PadButton && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//�J������]�ύX����߂�ꍇ�͗v�f�������ƈقȂ�̂ŁA���ɂ���s���ɂ���Đ��l�𒲐�
		if (selectInputType == CameraRota) {
			if (selectAction == CameraUpDown) { selectAction = GameInputManager::CameraUpRota; }
			else if (selectAction == CameraLeftRight) { selectAction = GameInputManager::CameraLeftRota; }
		}

		//�O�̓��͎�ނ�
		selectInputType--;

		//�V���ɑI������g�̐F��ύX
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}

	//�I���s���ύX
	if (selectAction > 0 && (DirectInput::GetInstance()->TriggerKey(DIK_UP) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP))) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Key && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == PadButton && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//�O�̍s����
		selectAction--;

		//�V���ɑI������g�̐F��ύX
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}
	else if (selectAction < (int)configSprites[selectInputType].size() - 1 && (DirectInput::GetInstance()->TriggerKey(DIK_DOWN) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN))) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Key && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == PadButton && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//���̍s����
		selectAction++;

		//�V���ɑI������g�̐F��ύX
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}

	//�ύX����I�����m��
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
	//���͐ݒ�I��
	else if (DirectInput::GetInstance()->TriggerKey(DIK_TAB) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_B)) {
		isInputConfigEnd = true;
	}
}

void ActionInputConfig::InputChangeModeUpdate()
{
	//�L�[
	if (selectInputType == Key) {
		//�L�[���蓖�čX�V
		BYTE newKey;
		if (DirectInput::GetInstance()->GetTriggerKey(newKey)) {
			//���̍s���Ɣ���Ă��Ȃ���ΐi��
			if (GameInputManager::ChangeInputKey((GameInputManager::InputAction)selectAction, newKey)) {
				//�w�肵���L�[�̃e�N�X�`�������邩
				if (!Sprite::GetIsTextureName("key_" + std::to_string(newKey))) {
					return;
				}
				//�e�N�X�`�����蓖��
				configSprites[Key][selectAction]->SetTexture("key_" + std::to_string(newKey));

				//�I���ɖ߂�
				phase = Phase::SelectMode;
			}
		}
	}
	//�{�^��
	else if (selectInputType == PadButton) {
		//�{�^�����蓖�čX�V
		int newButton;
		if (XInputManager::GetInstance()->GetTriggerButton(newButton)) {
			//���̍s���Ɣ���Ă��Ȃ���ΐi��
			if (GameInputManager::ChangeInputPadButton((GameInputManager::InputAction)selectAction, newButton)) {
				//�w�肵���{�^���̃e�N�X�`�������邩
				if (!Sprite::GetIsTextureName("pad_" + std::to_string(newButton))) {
					return;
				}

				//�e�N�X�`�����蓖��
				configSprites[PadButton][selectAction]->SetTexture("pad_" + std::to_string(newButton));

				//�I���ɖ߂�
				phase = Phase::SelectMode;
			}
		}
	}
	//�J������]
	else if (selectInputType == CameraRota) {
		//�I�������J������]�̉�]�������t�ɂ���
		if (selectAction == CameraUpDown) {
			GameInputManager::SetIsCameraRotaXReverse(!(GameInputManager::GetIsCameraRotaXReverse()));

			//�e�N�X�`���ύX
			if (!GameInputManager::GetIsCameraRotaXReverse()) { configSprites[CameraRota][selectAction]->SetTexture("normal"); }
			else { configSprites[CameraRota][selectAction]->SetTexture("reverse"); }
		}
		else if (selectAction == CameraLeftRight) {
			GameInputManager::SetIsCameraRotaYReverse(!(GameInputManager::GetIsCameraRotaYReverse()));

			//�e�N�X�`���ύX
			if (!GameInputManager::GetIsCameraRotaYReverse()) { configSprites[CameraRota][selectAction]->SetTexture("normal"); }
			else { configSprites[CameraRota][selectAction]->SetTexture("reverse"); }
		}

		//�I���ɖ߂�
		phase = Phase::SelectMode;
	}
}
