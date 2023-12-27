#include "SceneChangeDirection.h"
#include "WindowApp.h"

void SceneChangeDirection::Create()
{
	fadeSprite = Sprite::Create("white", {}, {});
	fadeSprite->SetSize({ float(WindowApp::GetWindowWidth()), float(WindowApp::GetWindowHeight()) });
	isLoaded = true;
}

void SceneChangeDirection::Init()
{
	isDirectionEnd = false;
	isLoaded = false;
	isFadein = false;
	isPlayingDirection = false;
	frame = 0;

	PlayFadeIn();
}

void SceneChangeDirection::Update()
{
	if (isDirectionEnd) { return; }

	float rate = float(frame) / float(CHANGE_FADE_FRAME);
	fadeSprite->SetColor({ 0,0,0,rate });
	fadeSprite->Update();

	if (isFadein) {
		frame--;
		if (frame <= 0) {
			isPlayingDirection = false;
		}
	}
	else {
		frame++;
		if (frame > CHANGE_FADE_FRAME) {
			isDirectionEnd = true;
			isPlayingDirection = false;
		}
	}
}

void SceneChangeDirection::Draw()
{

	float rate = float(frame) / float(CHANGE_FADE_FRAME);
	if (rate < 0.01f) { return; }
	fadeSprite->Draw();
}

void SceneChangeDirection::PlayFadeIn()
{
	if (isPlayingDirection) { return; }
	isDirectionEnd = false;
	isFadein = true;
	frame = CHANGE_FADE_FRAME;
	isPlayingDirection = true;
}

void SceneChangeDirection::PlayFadeOut()
{
	if (isPlayingDirection) { return; }
	isDirectionEnd = false;
	isFadein = false;
	frame = 0;
	isPlayingDirection = true;
}

void SceneChangeDirection::Delete()
{
	fadeSprite.reset();
}
