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
}

void LockonUI::Update()
{
	if (!isDraw) { return; }

	float scaleRate = float(frame) / float(SCALE_ANIM_FRAME);

	//スケールと色の変更
	if (frame < SCALE_ANIM_FRAME) {
		const float beginScale = 7.0f;
		float nowScale = Easing::InQuad(beginScale, 2.0f, scaleRate);
		lockOnObject->SetScale({ nowScale,nowScale,nowScale });
		lockOnObject->SetColor({ scaleRate,scaleRate,scaleRate,scaleRate });
	}

	//座標更新
	if (position != nullptr) { lockOnObject->SetPosition(*position); }

	//ビルボード化
	//ビューの逆行列
	DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&camera->GetEye());
	DirectX::XMVECTOR target = DirectX::XMLoadFloat3(&camera->GetTarget());
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&camera->GetUp());
	//Z軸計算
	DirectX::XMVECTOR axisZ = DirectX::XMVectorSubtract(target, eye);
	axisZ = DirectX::XMVector3Normalize(axisZ);
	//x軸計算
	DirectX::XMVECTOR axisX = DirectX::XMVector3Cross(up, axisZ);
	axisX = DirectX::XMVector3Normalize(axisX);
	//y軸計算
	DirectX::XMVECTOR axisY = DirectX::XMVector3Cross(axisZ, axisX);
	DirectX::XMMATRIX viewinv = DirectX::XMMatrixIdentity();
	viewinv.r[0] = axisX;
	viewinv.r[1] = axisY;
	viewinv.r[2] = axisZ;
	viewinv.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(lockOnObject->GetScale().x, lockOnObject->GetScale().y, lockOnObject->GetScale().z);
	DirectX::XMMATRIX rot = DirectX::XMMatrixIdentity();
	rot = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(lockOnObject->GetRotation().z));
	rot = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(lockOnObject->GetRotation().x));
	rot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(lockOnObject->GetRotation().y));
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(position->x, position->y, position->z);
	world *= viewinv;
	world *= scale;
	world *= rot;
	world *= trans;
	lockOnObject->SetMatWorld(world);
	lockOnObject->Update();

	//時間管理
	if (frame < DISPLAY_FRAME) {
		frame++;
	}
}

void LockonUI::Draw()
{
	if (isDraw) {
		lockOnObject->Draw(ObjectBase::DrawMode::add);
	}
}

void LockonUI::StartLockOnAnimation(const DirectX::XMFLOAT3 *enemyPos)
{
	frame = 0;
	position = enemyPos;
	isDraw = true;
}