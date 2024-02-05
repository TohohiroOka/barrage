#include "PauseScene.h"
#include "system/GameInputManager.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "scene/SceneManager.h"
#include "WindowApp.h"


void PauseScene::Init(bool isTitleScene)
{
	msDrawer = std::make_unique<MessageBoxDrawer>();
	
	if (isTitleScene) {
		choice.push_back(L"�Q�[�����Â���");
		choice.push_back(L"�Q�[���p�b�h�ݒ�");
		choice.push_back(L"�L�[�{�[�h�ݒ�");
	}
	else {
		choice.push_back(L"�Q�[�����Â���");
		choice.push_back(L"�Q�[���p�b�h�ݒ�");
		choice.push_back(L"�L�[�{�[�h�ݒ�");
		choice.push_back(L"�^�C�g���ɂ��ǂ�");
	}

	pauseQsys = std::make_unique<QuestionSystem>(choice);

	TextureManager::LoadTexture("pausetext", "Resources/SpriteTexture/pauseText.png");
	pauseText = Sprite::Create("pausetext", { 1500.f / 2.f,150.f }, { 0.5f,0.5f });
	pauseText->Update();

	aicKey = std::make_unique<ActionInputConfigKey>();
	aicPad = std::make_unique<ActionInputConfigPad>();

	TextureManager::LoadTexture("screenback", "Resources/SubTexture/white1x1.png");
	screenBack = Sprite::Create("screenback", {}, {});
	screenBack->SetSize({ float(WindowApp::GetWindowWidth()),float(WindowApp::GetWindowHeight()) });
	screenBack->SetColor({ 0,0,0,0.5f });
	screenBack->Update();

}

void PauseScene::Update()
{
	if (!isPause) { return; }

	//�I��������
	if (pauseQsys->GetIsEnd()) {
		SELECT userSelect = SELECT(pauseQsys->GetSelectNum());
		switch (userSelect)
		{
		case PauseScene::SELECT::SELECT_CONTINUE:
			//�^�C�g���͑����J�ڂ���̂ł����ł͂Ȃɂ����Ȃ�
			break;
		case PauseScene::SELECT::SELECT_CONFIG_PAD:
			//�R���t�B�O��ʂ̃t���O�����s
			isConfig = true;
			//�����[�h�t���O�����s
			isSelected = true;
			break;
		case PauseScene::SELECT::SELECT_CONFIG_KEY:
			//�R���t�B�O��ʂ̃t���O�����s
			isConfig = true;
			//�����[�h�t���O�����s
			isSelected = true;
			break;
		case PauseScene::SELECT::SELECT_TITLE:
			//�V�[���J�ڏ������s
			isChangeScene = true;
			break;
		default:
			break;
		}
	}

	if (isSelected) {
		//�R���t�B�O�X�V
		if (isConfig) {
			if (pauseQsys->GetSelectNum() == int(SELECT::SELECT_CONFIG_PAD)) {
				if (aicPad->GetIsInputConfigEnd()) {
					//�I��
					aicPad->Reset();
					isConfig = false;
					isSelected = false;
					pauseQsys->Init();
				}
				aicPad->Update();
			}
			else if (pauseQsys->GetSelectNum() == int(SELECT::SELECT_CONFIG_KEY)) {
				if (aicKey->GetIsInputConfigEnd()) {
					//�I��
					aicKey->Reset();
					isConfig = false;
					isSelected = false;
					pauseQsys->Init();
				}
				aicKey->Update();
			}
		}
		//���ʐݒ�X�V
		if (isVolume) {

		}
	}
	//�R���t�B�O�Ȃǂ̑����[�h�����s���Ă��Ȃ��Ƃ�
	else {
		//�|�[�X�I������
		if (oldPause && GameInputManager::TriggerInputAction(GameInputManager::Pause)) {
			isPause = false;
			pauseQsys->Init();
		}
		//�Q�[���ɖ߂�Ƃ��͑����|�[�Y�I��
		if (Select() && pauseQsys->GetSelectNum() == int(SELECT::SELECT_CONTINUE)) {
			isPause = false;
			pauseQsys->Init();
		}
	}

	pauseQsys->Update();
}

void PauseScene::Draw()
{
	if (!isPause) { return; }

	screenBack->Draw();

	pauseText->Draw();

	if (isConfig) {
		if (pauseQsys->GetSelectNum() == int(SELECT::SELECT_CONFIG_PAD)) {
			aicPad->Draw();
		}
		else if(pauseQsys->GetSelectNum() == int(SELECT::SELECT_CONFIG_KEY)) {
			aicKey->Draw();
		}
	}
	else if (isVolume) {

	}
	else {
		pauseQsys->Draw();
	}


}

void PauseScene::CheckPauseInput()
{
	oldPause = isPause;
	if (GameInputManager::TriggerInputAction(GameInputManager::Pause)){
		isPause = true;
	}
}

bool PauseScene::UpInput()
{
	bool result = false;
	result = DirectInput::GetInstance()->TriggerKey(DIK_W) ||
		DirectInput::GetInstance()->TriggerKey(DIK_UP) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP) ||
		XInputManager::GetInstance()->TriggerLeftStickY(true);
	return result;
}

bool PauseScene::DownInput()
{
	bool result = false;
	result = DirectInput::GetInstance()->TriggerKey(DIK_S) ||
		DirectInput::GetInstance()->TriggerKey(DIK_DOWN) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN) ||
		XInputManager::GetInstance()->TriggerLeftStickY(false);
	return result;
}

bool PauseScene::Select()
{
	return GameInputManager::TriggerInputAction(GameInputManager::Jump);
}

bool PauseScene::Back()
{
	return GameInputManager::TriggerInputAction(GameInputManager::Avoid_Blink_Dash);
}

bool PauseScene::CheckNowInputIsPad()
{
	bool result = XInputManager::GetInstance()->ControllerConnectCheck();

	return result;
}
