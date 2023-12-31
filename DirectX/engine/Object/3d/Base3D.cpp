#include "Base3D.h"
#include "Object/3d/collider/BaseCollider.h"
#include "Object/3d/collider/CollisionManager.h"
#include "Camera/Camera.h"
#include "Camera/LightCamera.h"
#include "Light/LightGroup.h"
#include "WindowApp.h"

using namespace DirectX;

Camera* Base3D::camera = nullptr;
LightCamera* Base3D::lightCamera = nullptr;
LightGroup* Base3D::light = nullptr;
TextureManager* Base3D::lightDepthTexture = nullptr;

void Base3D::Update()
{
	// 当たり判定更新
	if (collider) {
		collider->Update();
	}
}

void Base3D::UpdateWorldMatrix()
{
	if (isCalcWorldMat) { return; }
	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity(); // 変形をリセット
	matWorld *= matScale; // ワールド行列にスケーリングを反映
	matWorld *= matRot; // ワールド行列に回転を反映
	matWorld *= matTrans; // ワールド行列に平行移動を反映

	// 親オブジェクトがあれば
	if (parent != nullptr) {
		//親オブジェクトのワールド行列をかける
		matWorld *= parent->GetMatWorld();
	}
}

void Base3D::DeleteCollider()
{
	//コライダー解放
	if (collider) {
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

void Base3D::SetCollider(BaseCollider* _collider)
{
	_collider->SetObject(this);
	this->collider = _collider;
	// コリジョンマネージャに追加
	CollisionManager::GetInstance()->AddCollider(_collider);

	UpdateWorldMatrix();
	_collider->Update();
}

const XMFLOAT3 Base3D::GetWorldPosition()
{
	XMFLOAT3 worldpos;

	worldpos.x = matWorld.r[3].m128_f32[0];
	worldpos.y = matWorld.r[3].m128_f32[1];
	worldpos.z = matWorld.r[3].m128_f32[2];

	return worldpos;
}

const XMFLOAT2 Base3D::GetScreenPosition()
{
	XMFLOAT3 worldPos = GetWorldPosition();

	//ビューポート行列
	XMMATRIX matViewport = {
		{(float)WindowApp::GetWindowWidth() / 2, 0, 0, 0 },
		{0, -(float)WindowApp::GetWindowHeight() / 2, 0, 0},
		{0, 0, 1, 0},
		{(float)WindowApp::GetWindowWidth() / 2, (float)WindowApp::GetWindowHeight() / 2, 0, 1}
	};

	//ビュー行列とプロジェクション行列とビューポート行列を合成
	XMMATRIX matViewProjectionViewport =
		camera->GetView() * camera->GetProjection() * matViewport;

	//ワールド座標からスクリーン座標に変換
	worldPos = MatrixTransformWDivision(worldPos, matViewProjectionViewport);
	XMFLOAT2 screenPos = { worldPos.x, worldPos.y };

	return screenPos;
}

void Base3D::ColliderDraw()
{
	collider->Draw();
}
