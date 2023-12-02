#include "GameOver.h"

#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Scene/SceneManager.h"
#include "game/scene/Scene1.h"
#include "game/scene/TitleScene.h"
#include "cutscene/SceneChangeDirection.h"

void GameOver::Initialize()
{
	//ロードと生成
	Sprite::LoadTexture("gameover_gameover", "Resources/SpriteTexture/gameover.png");
	Sprite::LoadTexture("gameover_continue", "Resources/SpriteTexture/continue.png");
	Sprite::LoadTexture("gameover_exit", "Resources/SpriteTexture/exit.png");
	Sprite::LoadTexture("gameover_back", "Resources/SpriteTexture/selectBack.png");
	gameoverText = Sprite::Create("gameover_gameover", {}, {0.5f,0.5f});
	continueText = Sprite::Create("gameover_continue",	{},	{ 0.5f,0.5f });
	exitText = Sprite::Create("gameover_exit",			{},	{ 0.5f,0.5f });
	gameoverBack = Sprite::Create("gameover_back", {}, { 0.5f,0.5f });
	continueText->SetTexSize({ 448.f,64.f });
	exitText->SetTexSize({ 448.f,64.f });
	gameoverBack->SetSize({ 1600.f,900.f * 0.9f });
	//座標、色設定
	gameoverText->SetColor({ 1,1,1,0 });
	continueText->SetColor({ 1,1,1,0 });
	exitText->SetColor({ 1,1,1,0 });
	gameoverBack->SetColor({ 1,1,1,0 });
	gameoverText->SetPosition({ GAMEOVERTEXT_LT.x,GAMEOVERTEXT_LT.y });
	continueText->SetPosition({ CONTINUE_POS.x,CONTINUE_POS.y });
	exitText->SetPosition({ EXIT_POS.x,EXIT_POS.y });
	gameoverBack->SetPosition({ float(WINDOW_WIDTH) / 2.f,float(WINDOW_HEIGHT) / 2.f });
	continueText->SetAnchorpoint({ 0.5f,0.5f });
	exitText->SetAnchorpoint({ 0.5f,0.5f });
	
	//更新
	gameoverText->Update();
	continueText->Update();
	exitText->Update();

	choiceDrawer = std::make_unique<ChoiceEmphasisDrawer>();
	choiceDrawer->Initialize();
}

void GameOver::Update()
{
	if (phase == GAMEOVER_PHASE::PHASE_STANDBY) { return; }

	choiceDrawer->Update();

	frame++;

	float rate = 0;
	//シーン遷移用
	InterfaceScene* newScene = nullptr;

	switch (phase)
	{
	case GameOver::GAMEOVER_PHASE::PHASE_STANDBY:
		break;
	case GameOver::GAMEOVER_PHASE::PHASE_MODELFADE:
		if (frame > PLAYER_FADE_FRAME) {
			phase = GAMEOVER_PHASE::PHASE_UI_FADEIN;
			if (playerObject) { playerObject->SetColor({ 1,1,1,0 }); }
			frame = 0;
		}
		rate = float(frame) / float(PLAYER_FADE_FRAME);
		if (playerObject) { 
			playerObject->SetColor({ 1,1,1,1 - rate }); 
		}
		break;
	case GameOver::GAMEOVER_PHASE::PHASE_UI_FADEIN:
		if (frame > UI_FADEIN_FRAME) {
			phase = GAMEOVER_PHASE::PHASE_UI_SHOW;
			gameoverText->SetColor({ 1,1,1,1 });
			continueText->SetColor({ 1,1,1,1 });
			exitText->SetColor({ 1,1,1,1 });
			gameoverBack->SetColor({ 1,1,1,1 });
			frame = 0;
		}
		else {
			//フェードイン
			rate = float(frame) / float(UI_FADEIN_FRAME);
			gameoverText->SetColor({ 1,1,1,rate });
			continueText->SetColor({ 1,1,1,rate });
			exitText->SetColor({ 1,1,1,rate });
			gameoverBack->SetColor({ 1,1,1,rate });
		}
		break;
	case GameOver::GAMEOVER_PHASE::PHASE_UI_SHOW:
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
		//決定
		if (DirectInput::GetInstance()->TriggerKey(DIK_SPACE) ||
			XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A)) {
			choiceDrawer->PlayChoiseAnimation();
			phase = GAMEOVER_PHASE::PHASE_DECISION;
		}

		break;
	case GameOver::GAMEOVER_PHASE::PHASE_DECISION:
		if (choiceDrawer->IsChooseAnimEnd()) {
			SceneChangeDirection::PlayFadeOut();
		}
		if (SceneChangeDirection::IsDirectionEnd()) {
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
			if (newScene) { SceneManager::SetNextScene(newScene); }
		}
		break;
	default:
		break;
	}

	//選択肢強調表示
	switch (playerSelecting)
	{
	case GameOver::PlayerSelect::SELECT_CONTINUE:
		choiceDrawer->SetEmphasisPos(CONTINUE_POS.x, CONTINUE_POS.y, 600, 120);
		break;
	case GameOver::PlayerSelect::SELECT_EXIT:
		choiceDrawer->SetEmphasisPos(EXIT_POS.x, EXIT_POS.y, 600, 120);
		break;
	default:
		break;
	}

	//スプライト更新
	gameoverBack->Update();
	gameoverText->Update();
	continueText->Update();
	exitText->Update();
}

void GameOver::Draw()
{
	if (phase == GAMEOVER_PHASE::PHASE_UI_FADEIN ||
		phase == GAMEOVER_PHASE::PHASE_UI_SHOW ||
		phase == GAMEOVER_PHASE::PHASE_DECISION) {
		gameoverBack->Draw();
		gameoverText->Draw();
		continueText->Draw();
		exitText->Draw();
	}

	if (phase == GAMEOVER_PHASE::PHASE_UI_SHOW ||
		phase == GAMEOVER_PHASE::PHASE_DECISION) {
		choiceDrawer->Draw();
	}
}

void GameOver::StartGameOverUI()
{
	if (phase != GAMEOVER_PHASE::PHASE_STANDBY) { return; }
	phase = GAMEOVER_PHASE::PHASE_MODELFADE;
	frame = 0;
}

void GameOver::ResetGameOverUI()
{
	phase = GAMEOVER_PHASE::PHASE_STANDBY;
	frame = 0;
}

void GameOver::SetPlayerObject(Fbx* playerObject)
{
	this->playerObject = playerObject;
}