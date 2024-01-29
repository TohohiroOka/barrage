#include "TutorialPause.h"
#include "WindowApp.h"
#include "ui/TextManager.h"

TutorialPause::TutorialPause()
{
	//仮置き背景スプライト初期化
	backSprite = Sprite::Create("white1x1", { 0,0 }, {}, { 0,0,0,0.7f });
	backSprite->SetSize({ (float)WindowApp::GetWindowWidth(), (float)WindowApp::GetWindowHeight() });
}

TutorialPause::~TutorialPause()
{
}

void TutorialPause::Update()
{
	//ポーズ画面中でなければ抜ける
	if (!isPauseDraw) { return; }

	backSprite->Update();
}

void TutorialPause::Draw()
{
	//ポーズ画面中でなければ抜ける
	if (!isPauseDraw) { return; }

	backSprite->Draw();
}

void TutorialPause::PauseStart()
{
	//すでにポーズ画面中であれば抜ける
	if (isPauseDraw) { return; }

	//ポーズ画面中に設定
	isPauseDraw = true;

	//チュートリアル専用ポーズ画面のテキストと選択肢を表示
	TextManager::Instance()->ChoicesCreate(ChoicesData::ChoicesName::TUTORIAL_PAUSE_CHOICE);
}

void TutorialPause::PauseEnd()
{
	//ポーズ画面終了
	isPauseDraw = false;
	//選択肢の表示終了
	TextManager::Instance()->ChoicesDrawEnd();
}
