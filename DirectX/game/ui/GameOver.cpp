#include "GameOver.h"

#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Scene/SceneManager.h"
#include "game/scene/Scene1.h"
#include "game/scene/TitleScene.h"

void GameOver::Initialize()
{
	//ロードと生成
	Sprite::LoadTexture("gameover_gameover", "Resources/SpriteTexture/gameover.png");
	Sprite::LoadTexture("gameover_continue", "Resources/SpriteTexture/continue.png");
	Sprite::LoadTexture("gameover_exit", "Resources/SpriteTexture/exit.png");
	gameoverText = Sprite::Create("gameover_gameover",	{},	{ 512.f,64.f });
	continueText = Sprite::Create("gameover_continue",	{},	{ 448.f,64.f });
	exitText = Sprite::Create("gameover_exit",			{},	{ 448.f,64.f });
	gameoverText->SetTexSize({ 512.f,64.f });
	continueText->SetTexSize({ 448.f,64.f });
	exitText->SetTexSize({ 448.f,64.f });
	//座標、色設定
	gameoverText->SetColor({ 1,1,1,0 });
	continueText->SetColor({ 1,1,1,0 });
	exitText->SetColor({ 1,1,1,0 });
	gameoverText->SetPosition({ GAMEOVERTEXT_LT.x,GAMEOVERTEXT_LT.y });
	continueText->SetPosition({ CONTINUE_POS.x,CONTINUE_POS.y });
	exitText->SetPosition({ EXIT_POS.x,EXIT_POS.y });
	continueText->SetAnchorpoint({ 0.5f,0.5f });
	exitText->SetAnchorpoint({ 0.5f,0.5f });
	
	//更新
	gameoverText->Update();
	continueText->Update();
	exitText->Update();
}

void GameOver::Update()
{
	if (!isGameOver) { return; }

	if (frame < GAMEOVER_DIRECTION_FRAME) { 
		frame++; 
		//入力で演出スキップ
		if (DirectInput::GetInstance()->TriggerKey(DIK_SPACE) ||
			XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_B)) {
			frame = 299;
		}
	}



	float rate = float(frame) / float(GAMEOVER_DIRECTION_FRAME);

	//フェードイン
	gameoverText->SetColor({ 1,1,1,rate });
	continueText->SetColor({ 1,1,1,rate });
	exitText->SetColor({ 1,1,1,rate });

	//文字サイズを一度初期値に設定
	continueText->SetSize({ float(CONTINUE_EXIT_WIDTH),64.f });
	exitText->SetSize({ float(CONTINUE_EXIT_WIDTH),64.f });

	//選択中のスプライトを拡大
	switch (playerSelecting)
	{
	case GameOver::PlayerSelect::SELECT_CONTINUE:
		continueText->SetSize({ float(CONTINUE_EXIT_WIDTH) * 1.5f, 64.f * 1.5f });
		break;
	case GameOver::PlayerSelect::SELECT_EXIT:
		exitText->SetSize({ float(CONTINUE_EXIT_WIDTH) * 1.5f, 64.f * 1.5f });
		break;
	default:
		break;
	}

	//スプライト更新
	gameoverText->Update();
	continueText->Update();
	exitText->Update();

	if (frame < GAMEOVER_DIRECTION_FRAME) { return; }
	//演出終了後に入力を取得
	if (DirectInput::GetInstance()->TriggerKey(DIK_LEFT) ||
		XInputManager::GetInstance()->TriggerLeftStickX(true)) {
		if (playerSelecting != PlayerSelect::SELECT_CONTINUE) {
			playerSelecting = PlayerSelect(int(playerSelecting) - 1); 
		}
	}
	if (DirectInput::GetInstance()->TriggerKey(DIK_RIGHT) ||
		XInputManager::GetInstance()->TriggerLeftStickX(false)) {
		if (playerSelecting != PlayerSelect::SELECT_EXIT) {
			playerSelecting = PlayerSelect(int(playerSelecting) + 1);
		}
	}
	
	//シーン遷移
	if (DirectInput::GetInstance()->TriggerKey(DIK_SPACE) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_B)) {
		InterfaceScene* newScene = nullptr;
		switch (playerSelecting)
		{
		case GameOver::PlayerSelect::SELECT_CONTINUE:
			newScene = new Scene1;
			break;
		case GameOver::PlayerSelect::SELECT_EXIT:
			newScene = new TitleScene;
			break;
		default:
			break;
		}
		if(newScene){ SceneManager::SetNextScene(newScene); }
	}
}

void GameOver::Draw()
{
	if (!isGameOver) { return; }

	gameoverText->Draw();
	continueText->Draw();
	exitText->Draw();
}

void GameOver::StartGameOverUI()
{
	isGameOver = true;
}

void GameOver::ResetGameOverUI()
{
	isGameOver = false;
	frame = 0;
}