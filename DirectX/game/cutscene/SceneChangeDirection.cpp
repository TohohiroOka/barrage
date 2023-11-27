#include "SceneChangeDirection.h"
#include "WindowApp.h"

std::unique_ptr<Sprite> SceneChangeDirection::fadeSprite;
bool SceneChangeDirection::isDirectionEnd = false;
bool SceneChangeDirection::isLoaded = false;
bool SceneChangeDirection::isFadein = false;
bool SceneChangeDirection::isPlayingDirection = false;
const int SceneChangeDirection::CHANGE_FADE_FRAME = 60;
int SceneChangeDirection::frame = 0;

void SceneChangeDirection::Create()
{
	if (!isLoaded) {
		Sprite::LoadTexture("fadetex", "Resources/SubTexture/white1x1.png", false);
		fadeSprite = Sprite::Create("fadetex", {}, {});
		fadeSprite->SetSize({ float(WindowApp::GetWindowWidth()), float(WindowApp::GetWindowHeight()) });
		isLoaded = true;
	}
}

void SceneChangeDirection::Init()
{
	Create();
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