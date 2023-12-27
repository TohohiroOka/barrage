#include "BaseDefeatDirection.h"
#include "WindowApp.h"
#include "scene/TitleScene.h"
#include "Scene/SceneManager.h"

#include "Math/Easing/Easing.h"

void BaseDefeatDirection::Init()
{
	TextureManager::LoadTexture("victorytext", "Resources/SpriteTexture/victory.png");
	victoryTextUpper = Sprite::Create("victorytext", {}, { 0.5,0.5 });
	victoryTextUpper->SetTexSize({ 512,64.f });
	victoryTextUpper->SetSize({ 512.f * 1.25f,64.f * 1.25 });
	victoryTextUpper->SetPosition({ float(WindowApp::GetWindowWidth()) / 2.f,float(WindowApp::GetWindowHeight()) / 2.f});
	victoryTextUpper->Update();
}

void BaseDefeatDirection::Update()
{
	if (!isCommonDirecting) { return; }
	if (!isCommonEnded){ commonDirectFrame++; }
	if (isFadeInEnd && isDisplayEnd && isFadeOutEnd) { 
		isCommonEnded = true; 
	}
	//‹¤’Ê‚ÌŸ—˜‰‰o‚ÌXV


	if (!isFadeInEnd) {
		float t = float(commonDirectFrame) / float(TEXT_FADEIN_MAXFRAME);
		float rate = Easing::InCubic(1.5f, 1.f, t);
		victoryTextUpper->SetSize({ 512.f * rate,64.f * rate });
		victoryTextUpper->SetColor({ 1.0f,1.0f,1.0f,1.0f * t });
		victoryTextUpper->Update();
		if (commonDirectFrame > TEXT_FADEIN_MAXFRAME) {
			commonDirectFrame = 0;
			isFadeInEnd = true;
		}
	}
	else if (!isDisplayEnd) {
		if (commonDirectFrame > TEXT_DISPLAY_MAXFRAME) {
			commonDirectFrame = 0;
			isDisplayEnd = true;
		}
	}
	else if (!isFadeOutEnd) {
		float t = float(commonDirectFrame) / float(TEXT_FADEOUT_MAXFRAME);
		float rate = Easing::OutCubic(1.f, 1.5f, t);
		float alphaRate = Easing::OutCubic(1.f, 0.f, t);
		victoryTextUpper->SetSize({ 512.f * rate,64.f * rate });
		victoryTextUpper->SetColor({ 1.0f,1.0f,1.0f,alphaRate });
		victoryTextUpper->Update();
		if (commonDirectFrame > TEXT_FADEOUT_MAXFRAME) {
			commonDirectFrame = 0;
			isFadeOutEnd = true;
		}
	}
	
}

void BaseDefeatDirection::Draw()
{
}

void BaseDefeatDirection::Draw2D()
{
	if (isCommonDirecting) { victoryTextUpper->Draw(); }
}

void BaseDefeatDirection::Reset()
{
	frame = 0;
	isDirecting = false;
	isEnded = false;
	commonDirectFrame = 0;
	isCommonDirecting = false;
	isCommonEnded = false;
}

void BaseDefeatDirection::InEmitter(ParticleManager* ptr, ParticleGrainState& pgState)
{
	ptr->Add(pgState.aliveFrame,
		pgState.pos,
		pgState.vel,
		pgState.acc,
		pgState.s_scale,
		pgState.e_scale,
		pgState.s_color,
		pgState.e_color);
}

void BaseDefeatDirection::StartCommonDirection()
{
	if(!isCommonDirecting){ 
		commonDirectFrame = 0;
		isCommonDirecting = true; 
	}
}

void BaseDefeatDirection::StartDefeatDirection(const DirectX::XMFLOAT3 &bossPosition)
{
	//‚·‚Å‚ÉÄ¶’†‚Ìê‡‚Í–³Œø
	if (isDirecting) { return; }

	isEnded = false;
	isDirecting = true;
	this->bossPosition = bossPosition;
	frame = 0;

	commonDirectFrame = 0;
	isCommonDirecting = false;
	isCommonEnded = false;
}
