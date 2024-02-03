#include "PauseScene.h"
#include "system/GameInputManager.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "scene/SceneManager.h"
#include "WindowApp.h"


void PauseScene::Init()
{
	msDrawer = std::make_unique<MessageBoxDrawer>();
	
	choice.push_back(L"ゲームをつづける");
	choice.push_back(L"コンフィグをひらく");
	choice.push_back(L"タイトルにもどる");

	pauseQsys = std::make_unique<QuestionSystem>(choice);

	TextureManager::LoadTexture("pausetext", "Resources/SpriteTexture/pauseText.png");
	pauseText = Sprite::Create("pausetext", { 1500.f / 2.f,150.f }, { 0.5f,0.5f });
	pauseText->Update();

	aic = std::make_unique<ActionInputConfigKey>();

	TextureManager::LoadTexture("screenback", "Resources/SubTexture/white1x1.png");
	screenBack = Sprite::Create("screenback", {}, {});
	screenBack->SetSize({ float(WindowApp::GetWindowWidth()),float(WindowApp::GetWindowHeight()) });
	screenBack->SetColor({ 0,0,0,0.5f });
	screenBack->Update();

}

void PauseScene::Update()
{
	if (!isPause) { return; }

	//選択肢判定
	if (pauseQsys->GetIsEnd()) {
		SELECT userSelect = SELECT(pauseQsys->GetSelectNum());
		switch (userSelect)
		{
		case PauseScene::SELECT::SELECT_CONTINUE:
			//タイトルは即時遷移するのでここではなにもしない
			break;
		case PauseScene::SELECT::SELECT_CONFIG:
			//コンフィグ画面のフラグを実行
			isConfig = true;
			//他モードフラグを実行
			isSelected = true;
			
			break;
		case PauseScene::SELECT::SELECT_TITLE:
			//シーン遷移処理実行
			isChangeScene = true;
			break;
		default:
			break;
		}
	}

	if (isSelected) {
		//コンフィグ更新
		if (isConfig) {
			//コンフィグ終了
			if (aic->GetIsInputConfigEnd()) {
				aic->Reset();
				isConfig = false;
				isSelected = false;
				pauseQsys->Init();
			}
			aic->Update();
		}
		//音量設定更新
		if (isVolume) {

		}
	}
	//コンフィグなどの他モードを実行していないとき
	else {
		//ポース終了判定
		if (DirectInput::GetInstance()->TriggerKey(DIK_TAB) ||
			XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_START)) {
			isPause = false;
			pauseQsys->Init();
		}
		//ゲームに戻るときは即時ポーズ終了
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
		aic->Draw();
	}
	else if (isVolume) {

	}
	else {
		pauseQsys->Draw();
	}


}

void PauseScene::InPause()
{
	if (DirectInput::GetInstance()->TriggerKey(DIK_TAB) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_START)) {
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
