#include "ActionInputConfigPad.h"

ActionInputConfigPad::ActionInputConfigPad()
	: ActionInputConfigBase()
{
	for (int i = Config; i < InputTypeNum; i++) {
		//�e���͎�ނ̗v�f��
		int loopNum = 0;
		if (i == CameraRota) { loopNum = CameraRotaTypeNum; }
		else { loopNum = GameInputManager::Lockon + 1; }

		for (int j = 0; j < loopNum; j++) {
			//���̓X�v���C�g������
			std::string name;
			const DirectX::XMFLOAT2 leftTopPos = { 940, 75 };
			const DirectX::XMFLOAT2 posInterval = { 250, 55 };
			DirectX::XMFLOAT2 pos = { leftTopPos.x + (float)i * posInterval.x, leftTopPos.y + (float)j * posInterval.y };
			DirectX::XMFLOAT4 color = normalColor;
			if (i == Config) {
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

	//�I�𒆂̃X�v���C�g�̘g�̐F�����F������
	frameSprites[selectInputType][selectAction]->SetColor(selectColor);

	//�����X�v���C�g������
	infoSprites[InfoName::Select_Button] = Sprite::Create("pad_" + std::to_string(GameInputManager::GetPadInputActionData(GameInputManager::Select).padButton), { 1220, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Select_Text] = Sprite::Create("select", { 1280, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Back_Button] = Sprite::Create("pad_" + std::to_string(GameInputManager::GetPadInputActionData(GameInputManager::Back).padButton), { 1355, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Back_Text] = Sprite::Create("back", { 1415, 780 }, { 0.5f, 0.5f });
	infoSprites[InfoName::Button_Choice_Text] = Sprite::Create("buttonSelect", { 1305, 780 }, { 0.5f, 0.5f });
}

ActionInputConfigPad::~ActionInputConfigPad()
{
}

void ActionInputConfigPad::Reset()
{
	ActionInputConfigBase::Reset();

	//�t���[���X�v���C�g�̐F��߂�
	for (int i = 0; i < frameSprites.size(); i++) {
		for (int j = 0; j < frameSprites[i].size(); j++) {
			DirectX::XMFLOAT4 color = normalColor;
			if (i == Config && !GameInputManager::GetPadInputActionData(j).isChangeInput) { color = noChangeColor; }
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

void ActionInputConfigPad::SelectModeUpdate()
{
	//�I����͌`�ԕύX
	if (selectInputType < CameraRota && (DirectInput::GetInstance()->TriggerKey(DIK_D) || DirectInput::GetInstance()->TriggerKey(DIK_RIGHT) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_RIGHT) || XInputManager::GetInstance()->TriggerLeftStickX(false))) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Config && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
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
	else if (selectInputType > Config && (DirectInput::GetInstance()->TriggerKey(DIK_A) || DirectInput::GetInstance()->TriggerKey(DIK_LEFT) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_LEFT) || XInputManager::GetInstance()->TriggerLeftStickX(true))) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Config && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
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
	if (selectAction > 0 && (DirectInput::GetInstance()->TriggerKey(DIK_W) || DirectInput::GetInstance()->TriggerKey(DIK_UP) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP) || XInputManager::GetInstance()->TriggerLeftStickY(true))) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Config && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//�O�̍s����
		selectAction--;

		//�V���ɑI������g�̐F��ύX
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}
	else if (selectAction < (int)configSprites[selectInputType].size() - 1 && (DirectInput::GetInstance()->TriggerKey(DIK_S) || DirectInput::GetInstance()->TriggerKey(DIK_DOWN) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN) || XInputManager::GetInstance()->TriggerLeftStickY(false))) {
		//�I�𒆂̃X�v���C�g�̘g�̐F�����ɖ߂�
		DirectX::XMFLOAT4 returnColor = normalColor;
		if (selectInputType == Config && !GameInputManager::GetPadInputActionData(selectAction).isChangeInput) { returnColor = noChangeColor; }
		frameSprites[selectInputType][selectAction]->SetColor(returnColor);

		//���̍s����
		selectAction++;

		//�V���ɑI������g�̐F��ύX
		frameSprites[selectInputType][selectAction]->SetColor(selectColor);
	}

	//�ύX����I�����m��
	if (XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A)) {
		if (selectInputType == Config && GameInputManager::GetPadInputActionData(selectAction).isChangeInput) {
			configSprites[Config][selectAction]->DeleteTexture();

			phase = Phase::InputChangeMode;
		}
		else if (selectInputType == CameraRota) {
			phase = Phase::InputChangeMode;
		}
	}
	//���͐ݒ�I��
	else if (GameInputManager::TriggerInputAction(GameInputManager::Back) || GameInputManager::TriggerInputAction(GameInputManager::Pause)) {
		isInputConfigEnd = true;
	}
}

void ActionInputConfigPad::InputChangeModeUpdate()
{
	//�{�^��
	if (selectInputType == Config) {
		//�{�^�����蓖�čX�V
		int newButton;
		if (XInputManager::GetInstance()->GetTriggerButton(newButton)) {
			//���̍s���Ɣ���Ă��Ȃ���ΐi��
			if (GameInputManager::ChangeInputPadButton((GameInputManager::InputAction)selectAction, newButton)) {
				//�w�肵���{�^���̃e�N�X�`�������邩
				if (!TextureManager::GetIsTextureName("pad_" + std::to_string(newButton))) {
					return;
				}

				//�e�N�X�`�����蓖��
				configSprites[Config][selectAction]->SetTexture("pad_" + std::to_string(newButton));

				//�I���ɖ߂�
				phase = Phase::SelectMode;
			}
		}
	}
	//�J������]
	else if (selectInputType == CameraRota) {
		//�I�������J������]�̉�]�������t�ɂ���
		CameraRotationChange();

		//�I���ɖ߂�
		phase = Phase::SelectMode;
	}
}
