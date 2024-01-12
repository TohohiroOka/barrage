#include "LockonUI.h"

#include "Math/Easing/Easing.h"
#include "engine/Camera/Camera.h"

#include <DirectXMath.h>

LockonUI::LockonUI()
{
	lockonModel = Model::CreateFromOBJ("lockonSight");
	lockOnObject = Object3d::Create(lockonModel.get());
	lockOnObject->SetCalcWorldMat(true);
	lockOnObject->SetRotation({ 0,90,0 });
}

void LockonUI::Init(Camera* camera)
{
	this->camera = camera;
	
	TextureManager::LoadTexture("lockonSight", "Resources/SpriteTexture/lockonSight.png");
	lockonSprite = Sprite::Create("lockonSight", { 0,0 }, { 0.5f,0.5f });
}

void LockonUI::Update()
{
	if (!isDraw) { return; }

	float scaleRate = float(frame) / float(SCALE_ANIM_FRAME);

	//スケールと色の変更
	if (frame < SCALE_ANIM_FRAME) {
		const float beginScale = 7.0f;
		const float endScale = 1.0f;
		float nowScale = Easing::InQuad(beginScale, endScale, scaleRate);
		lockonSprite->SetSize({ 32.f * nowScale,32.f * nowScale });
		lockonSprite->SetColor({ scaleRate,scaleRate,scaleRate,scaleRate });
	}

	//座標更新
	if (position != nullptr) { lockonSprite->SetPosition(*position); }

	//時間管理
	if (frame < DISPLAY_FRAME) {
		frame++;
	}

	lockonSprite->Update();
}

void LockonUI::Draw()
{
	//if (isDraw) {
	//	lockOnObject->Draw(ObjectBase::DrawMode::add);
	//}

	lockonSprite->Draw();
}

void LockonUI::StartLockOnAnimation(const DirectX::XMFLOAT2 *enemyPos)
{
	frame = 0;
	position = enemyPos;
	isDraw = true;
}