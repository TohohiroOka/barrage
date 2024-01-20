#include "OKSprite.h"
#include "WindowApp.h"
#include "Math/Easing/Easing.h"

OKSprite::OKSprite()
{
	//スプライトのリソースのロード
	TextureManager::LoadTexture("OK", "Resources/SpriteTexture/OK.png");
	//スプライト生成
	okSprite = Sprite::Create("OK", { (float)WindowApp::GetWindowWidth() / 2, (float)WindowApp::GetWindowHeight() / 2 - 50 }, { 0.5f, 0.5f });

	//タイマー生成
	scaleChangeTimer = std::make_unique<Engine::Timer>();
}

OKSprite::~OKSprite()
{
}

void OKSprite::Update()
{
	//描画しない状態なら抜ける
	if (!isDraw) { return; }

	//スケール変更演出
	ScaleChange();

	okSprite->Update();
}

void OKSprite::Draw()
{
	//描画しない状態なら抜ける
	if (!isDraw) { return; }

	okSprite->Draw();
}

void OKSprite::DrawStart()
{
	//タイマー初期化
	scaleChangeTimer->Reset();

	//スケールを2倍にセットしておく
	spriteScale = 2.0f;

	//フラグリセット
	isDraw = true;
	isScaleChange = true;
}

void OKSprite::ScaleChange()
{
	//スケール変更演出中でなければ抜ける
	if (!isScaleChange) { return; }

	//タイマー更新
	const float scaleChangeTime = 15;
	scaleChangeTimer->Update();
	const float time = *scaleChangeTimer.get() / scaleChangeTime;

	//イージングでスケール変更
	spriteScale = Easing::OutBack(2.0f, 1.0f, time);
	okSprite->SetScale(spriteScale);

	//タイマーが指定した時間になったらスケール変更演出終了
	if (*scaleChangeTimer.get() >= scaleChangeTime) {
		isScaleChange = false;
	}
}
