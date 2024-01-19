#include "LockonUI.h"

#include "Math/Easing/Easing.h"
#include "engine/Camera/Camera.h"
#include "WindowApp.h"
#include "Math/Matrix4.h"

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

void LockonUI::Update(const DirectX::XMFLOAT3& enemyPos)
{
	if (!isDraw) { return; }

	float scaleRate = float(frame) / float(SCALE_ANIM_FRAME);

	//スケールと色の変更
	if (frame < SCALE_ANIM_FRAME) {
		const float beginScale = 7.0f;
		const float endScale = 1.0f;
		float nowScale = Easing::InQuad(beginScale, endScale, scaleRate);
		lockonSprite->SetSize({ 24.f * nowScale,24.f * nowScale });
		lockonSprite->SetColor({ 1,1,1,scaleRate });
	}

	//座標更新
	//ビュー行列と射影行列
	DirectX::XMMATRIX matview = camera->GetView(), matProjection = camera->GetProjection();
	//ビューポート行列
	DirectX::XMMATRIX matViewport = {
		{(float)WindowApp::GetWindowWidth() / 2, 0, 0, 0 },
		{0, -(float)WindowApp::GetWindowHeight() / 2, 0, 0},
		{0, 0, 1, 0},
		{(float)WindowApp::GetWindowWidth() / 2, (float)WindowApp::GetWindowHeight() / 2, 0, 1}
	};
	//ビューと射影を計算
	DirectX::XMFLOAT3 tmp = enemyPos;
	tmp.y = tmp.y + MODEL_Y_OFFSET;
	tmp = transform(tmp, matview);
	tmp = transform(tmp, matProjection);
	tmp.x /= tmp.z;
	tmp.y /= tmp.z;
	tmp.z /= tmp.z;
	DirectX::XMFLOAT3 screenPos = transform(tmp, matViewport);

	lockonSprite->SetPosition({ screenPos.x,screenPos.y });


	//時間管理
	if (frame < DISPLAY_FRAME) {
		frame++;
	}

	lockonSprite->Update();
}

void LockonUI::Draw()
{
	if (isDraw) {
		lockonSprite->Draw();
	}

}

void LockonUI::StartLockOnAnimation()
{
	frame = 0;
	isDraw = true;
}