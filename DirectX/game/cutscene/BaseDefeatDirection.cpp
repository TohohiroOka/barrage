#include "BaseDefeatDirection.h"
#include "WindowApp.h"

#include "Math/Easing/Easing.h"

void BaseDefeatDirection::Init()
{
	Sprite::LoadTexture("victorytext", "Resources/SpriteTexture/victory.png");
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
	if (commonDirectFrame >= COMMON_DIRECTION_MAXFRAME) { isCommonEnded = true; }
	//���ʂ̏������o�̍X�V


	if (commonDirectFrame < 30) {
		float t = float(commonDirectFrame) / 30.f;
		float rate = Easing::InCubic(1.5f, 1.f, t);
		victoryTextUpper->SetSize({ 512.f * rate,64.f * rate });
		victoryTextUpper->SetColor({ 1.0f,1.0f,1.0f,1.0f * t });
		victoryTextUpper->Update();
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

void BaseDefeatDirection::InEmitter(Emitter* ptr, ParticleGrainState& pgState)
{
	ptr->InEmitter(pgState.aliveFrame,
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
	//���łɍĐ����̏ꍇ�͖���
	if (isDirecting) { return; }

	isEnded = false;
	isDirecting = true;
	this->bossPosition = bossPosition;
	frame = 0;

	commonDirectFrame = 0;
	isCommonDirecting = false;
	isCommonEnded = false;
}
