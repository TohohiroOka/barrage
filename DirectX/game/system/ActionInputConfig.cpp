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
	//���u���w�i�p�e�N�X�`��
	Sprite::LoadTexture("white1x1", "Resources/SubTexture/white1x1.png");

	//�t���[���e�N�X�`���ǂݍ���
	Sprite::LoadTexture("inputFrame", "Resources/SpriteTexture/inputFrame.png");

	//�s�����e�N�X�`���ǂݍ���
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
	//������
	Initialize();
}

ActionInputConfig::~ActionInputConfig()
{
}

void ActionInputConfig::Initialize()
{
	//���u���w�i�X�v���C�g������
	backSprite = std::make_unique<Sprite>();
	backSprite->Initialize("white1x1", { 0,0 }, {}, { 0,0,0,0.7f });
	backSprite->SetSize({ (float)WindowApp::GetWindowWidth(), (float)WindowApp::GetWindowHeight() });

	//�s�����X�v���C�g������
	for (int i = 0; i < actionNameSprites.size(); i++) {
		actionNameSprites[i] = std::make_unique<Sprite>();
		const std::string name = "action_" + std::to_string(i);
		const DirectX::XMFLOAT2 pos = { 100, 100 + (float)i * 55 };
		actionNameSprites[i]->Initialize(name, pos, { 0, 0.5f });
	}

	//���̓X�v���C�g������
	for (int i = 0; i < configSprites[0].size(); i++) {
		configSprites[0][i] = std::make_unique<Sprite>();
		const std::string name = "key_" + std::to_string(GameInputManager::GetKeyInputActionData(i).key);
		const DirectX::XMFLOAT2 pos = { 820, 100 + (float)i * 55 };
		DirectX::XMFLOAT4 color = normalColor;
		if (!GameInputManager::GetKeyInputActionData(i).isChangeInput) { color = noChangeColor; }
		configSprites[0][i]->Initialize(name, pos, { 0.5f, 0.5f }, color);

		//�t���[���X�v���C�g������
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

		//�t���[���X�v���C�g������
		frameSprites[1][i] = std::make_unique<Sprite>();
		frameSprites[1][i]->Initialize("inputFrame", pos, { 0.5f, 0.5f }, color);
	}

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
			if (i == 0 && !GameInputManager::GetKeyInputActionData(j).isChangeInput) { color = noChangeColor; }
			else if (i == 1 && !GameInputManager::GetPadInputActionData(j).isChangeInput) { color = noChangeColor; }
			frameSprites[i][j]->SetColor(color);
		}
	}

	//�I�𒆂̃X�v���C�g�̘g�̐F�����F������
	frameSprites[selectInputType][selectAction]->SetColor(selectColor);
}

void ActionInputConfig::Select()
{
	//�I����͌`�ԕύX
	if (DirectInput::GetInstance()->TriggerKey(DIK_RIGHT) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_RIGHT)) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == 0 && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == 1 && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		selectInputType++;
		selectInputType = min(selectInputType, 1);

		//�V���ɑI������g�̐F��ύX
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}
	else if (DirectInput::GetInstance()->TriggerKey(DIK_LEFT) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_LEFT)) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == 0 && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == 1 && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		selectInputType--;
		selectInputType = max(selectInputType, 0);

		//�V���ɑI������g�̐F��ύX
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}

	//�I���s���ύX
	if (DirectInput::GetInstance()->TriggerKey(DIK_UP) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP)) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == 0 && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == 1 && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		selectAction--;
		selectAction = max(selectAction, 0);

		//�V���ɑI������g�̐F��ύX
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}
	else if (DirectInput::GetInstance()->TriggerKey(DIK_DOWN) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN)) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == 0 && !GameInputManager::GetKeyInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		else if (selectInputType == 1 && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		selectAction++;
		selectAction = min(selectAction, GameInputManager::InputActionNum - 1);

		//�V���ɑI������g�̐F��ύX
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}

	//�ύX����I�����m��
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
	//���͐ݒ�I��
	else if (DirectInput::GetInstance()->TriggerKey(DIK_F1) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_B)) {
		isInputConfigEnd = true;
	}
}

void ActionInputConfig::InputChange()
{
	//�L�[or�{�^���ύX�̏ꍇ
	if (selectAction <= GameInputManager::InputActionNum - 1) {
		//�L�[
		if (selectInputType == 0) {
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
					configSprites[0][selectAction]->SetTexture("key_" + std::to_string(newKey));

					//�I���ɖ߂�
					phase = Phase::SelectMode;
				}
			}
		}
		//�{�^��
		else if (selectInputType == 1) {
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
					configSprites[1][selectAction]->SetTexture("pad_" + std::to_string(newButton));

					//�I���ɖ߂�
					phase = Phase::SelectMode;
				}
			}
		}
	}
}
