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
	TextureManager::LoadTexture("gameover_gameover", "Resources/SpriteTexture/gameover.png");
	TextureManager::LoadTexture("gameover_continue", "Resources/SpriteTexture/continue.png");
	TextureManager::LoadTexture("gameover_exit", "Resources/SpriteTexture/exit.png");
	TextureManager::LoadTexture("gameover_back", "Resources/SpriteTexture/selectBack.png");
	gameoverText = Sprite::Create("gameover_gameover", {}, {0.5f,0.5f});
	continueText = Sprite::Create("gameover_continue",	{},	{ 0.5f,0.5f });
	exitText = Sprite::Create("gameover_exit",			{},	{ 0.5f,0.5f });
	gameoverBack = Sprite::Create("gameover_back", {}, { 0.5f,0.5f });
	continueText->SetTexSize({ 448.f,64.f });
	exitText->SetTexSize({ 448.f,64.f });
	gameoverBack->SetSize({ 1600.f,900.f * 0.9f });
	//座標、色設定
	gameoverText->SetColor({ 1,1,1,1 });
	continueText->SetColor({ 1,1,1,0 });
	exitText->SetColor({ 1,1,1,0 });
	gameoverBack->SetColor({ 1,1,1,0 });
	gameoverText->SetPosition({ GAMEOVERTEXT_LT.x,GAMEOVERTEXT_LT.y });
	continueText->SetPosition({ CONTINUE_POS.x,CONTINUE_POS.y });
	exitText->SetPosition({ EXIT_POS.x,EXIT_POS.y });
	gameoverBack->SetPosition({ float(WINDOW_WIDTH) / 2.f,float(WINDOW_HEIGHT) / 2.f });
	continueText->SetAnchorpoint({ 0.5f,0.5f });
	exitText->SetAnchorpoint({ 0.5f,0.5f });

	//質問システム
	std::vector<std::wstring> q;
	q.push_back(L"リトライ");
	q.push_back(L"ギブアップ");
	
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
			frame = 0;
		}
		break;
	case GameOver::GAMEOVER_PHASE::PHASE_UI_FADEIN:
		Audio::Instance()->SoundPlayWava(Sound::SoundName::gameover, false, 0.02f);
		//グリッジ的なテキスト表示
		if (frame % 3 == 0) {
			if (isWide) {
				isWide = false;
				gameoverText->SetSize({ 672 * glitchAnimScaleWide * RandomFloat(1.f,2.f)  ,64 * glitchAnimScaleSmall });
			}
			else {
				isWide = true;
				gameoverText->SetSize({ 672 * glitchAnimScaleSmall ,64 * glitchAnimScaleWide * RandomFloat(1.f,2.f) });
			}
		}
		if (frame > UI_FADEIN_FRAME) {
			phase = GAMEOVER_PHASE::PHASE_UI_SHOW;
			gameoverText->SetSize({ 672,64 });
			gameoverText->SetColor({ 1,1,1,1 });
			frame = 0;
		}
		break;
	case GameOver::GAMEOVER_PHASE::PHASE_UI_SHOW:
		//決定
		if (frame > 120) {
			frame = 0;
			SceneChangeDirection::Instance()->PlayFadeOut();
			phase = GAMEOVER_PHASE::PHASE_DECISION;
		}
		break;
	case GameOver::GAMEOVER_PHASE::PHASE_DECISION:
		if (SceneChangeDirection::Instance()->IsDirectionEnd()) {
			newScene = new Scene1;
			if (newScene) { SceneManager::SetNextScene(newScene); }
		}
		break;
	default:
		break;
	}

	if (int(phase) >= 3 &&  frame % 3 == 0) { 
		gameoverText->SetPosition({ GAMEOVERTEXT_LT.x + 
			RandomFloat(0.f,5.f) * RandomSign()
			,GAMEOVERTEXT_LT.y + 
			RandomFloat(0.f,5.f) * RandomSign()
			});
	}

	//スプライト更新
	gameoverText->Update();
}

void GameOver::Draw()
{
	if (phase == GAMEOVER_PHASE::PHASE_UI_FADEIN ||
		phase == GAMEOVER_PHASE::PHASE_UI_SHOW ||
		phase == GAMEOVER_PHASE::PHASE_DECISION) {
		gameoverText->Draw();
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