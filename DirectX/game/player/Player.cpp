#include "Player.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "Object/2d/DebugText.h"
#include "Object/3d/collider/SphereCollider.h"
#include "Object/3d/collider/CollisionManager.h"
#include "Object/3d/collider/CollisionAttribute.h"
#include "Math/Vector2.h"
#include "game/camera/GameCamera.h"

using namespace DirectX;

const float Player::moveSpeedMax = 1.5f;
const float Player::dashSpeedMax = 2.0f;

Player::Player(GameCamera* gameCamera)
{
	this->gameCamera = gameCamera;

	model = Model::CreateFromOBJ("NormalCube");
	object = Object3d::Create(model.get());
	pos = { 100.0f,200.0f,100.0f };
	moveVec = { 0.0f,0.0f,0.0f };

	// コライダーの追加
	float radius = 0.6f;
	object->SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	object->GetCollider()->SetAttribute(COLLISION_ATTR_ALLIES);

	//連続ジャンプ可能回数設定
	jumpMaxNum = 2;
}

void Player::Update()
{
	Move();
	if (!onGround) {
		// 落下処理
		Fall();
	}

	Jump();
	Collider();

	object->Update();
}

void Player::Draw()
{
	object->Draw();
}

void Player::Move()
{
	DirectInput* input = DirectInput::GetInstance();

	Vector2 raidan = Vector2(XMConvertToRadians(moveRota), XMConvertToRadians(moveRota + 90));

	//ダッシュ
	Dash();

	//キー入力
	{
		//右入力
		if (input->PushKey(DIK_D)) {
			moveVec.x += moveSpeed * cosf(raidan.x);
			moveVec.z += moveSpeed * cosf(raidan.y);
		}
		//左入力
		if (input->PushKey(DIK_A)) {
			moveVec.x -= moveSpeed * cosf(raidan.x);
			moveVec.z -= moveSpeed * cosf(raidan.y);
		}
		//下入力
		if (input->PushKey(DIK_W)) {
			moveVec.x += moveSpeed * cosf(XMConvertToRadians(360.0f - moveRota + 90));
			moveVec.z += moveSpeed * cosf(XMConvertToRadians(360.0f - moveRota));
		}
		//上入力
		if (input->PushKey(DIK_S)) {
			moveVec.x -= moveSpeed * cosf(XMConvertToRadians(360.0f - moveRota + 90));
			moveVec.z -= moveSpeed * cosf(XMConvertToRadians(360.0f - moveRota));
		}
	}

	//コントローラー入力
	{
		//ある程度スティックを傾けないと判定しない
		const float moveStickIncline = 0.3f;
		const XMFLOAT2 padIncline = XInputManager::GetInstance()->GetPadLStickIncline();
		if (!(fabsf(padIncline.x) >= moveStickIncline ||
			fabsf(padIncline.y) >= moveStickIncline)) {
			return;
		}

		
		Vector3 vec{};
		vec.x = XInputManager::GetInstance()->GetPadLStickIncline().x * moveSpeed;
		vec.z = XInputManager::GetInstance()->GetPadLStickIncline().y * moveSpeed;

		/*const float stickRadian = XMConvertToRadians(XInputManager::GetInstance()->GetPadLStickAngle());
		Vector3 stickVec{};
		stickVec.x = vec.x * cosf(stickRadian) - vec.z * sinf(stickRadian);
		stickVec.z = vec.x * sinf(stickRadian) + vec.z * cosf(stickRadian);*/

		const float cameraRotaRadian = XMConvertToRadians(-gameCamera->GetCameraRota().x);
		Vector3 cameraVec{};
		cameraVec.x = vec.x * cosf(cameraRotaRadian) - vec.z * sinf(cameraRotaRadian);
		cameraVec.z = vec.x * sinf(cameraRotaRadian) + vec.z * cosf(cameraRotaRadian);

		moveVec = cameraVec;
	}

	
}

void Player::Dash()
{
	//ダッシュ入力があった場合に、移動スピードを速くしていく
	if (DirectInput::GetInstance()->PushKey(DIK_Z) || XInputManager::GetInstance()->PushButton(XInputManager::PAD_B)) {
		moveSpeed = dashSpeedMax;
	}
	//入力がない場合は、元の移動スピードに戻していく
	else {
		moveSpeed = moveSpeedMax;
	}
}

void Player::Fall()
{
	// 下向き加速度
	const float fallAcc = -0.4f;
	const float fallVYMin = -10.0f;
	// 加速
	fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
	// 移動
	moveVec.y = fallV.m128_f32[1];
}

void Player::Jump()
{
	//ジャンプ回数が連続ジャンプ可能回数を超えていたら抜ける
	if (jumpCount >= jumpMaxNum) { return; }
	//ジャンプ入力がなければ抜ける
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_SPACE) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A))) { return; }

	onGround = false;
	const float jumpVYFist = 8.0f;
	fallV = { 0, jumpVYFist, 0, 0 };
	jumpCount++; //ジャンプ回数を増やす
}

void Player::Collider()
{
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(object->GetCollider());
	assert(sphereCollider);

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QUERY_HIT& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値
			const float threshold = cosf(XMConvertToRadians(90.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};
	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	pos.x += callback.move.m128_f32[0];
	pos.y += callback.move.m128_f32[1];
	pos.z += callback.move.m128_f32[2];

	{
		//	// 球の上端から球の下端までのレイキャスト
		//	Ray ray;
		//	ray.start = sphereCollider->center;
		//	ray.start.m128_f32[1] += sphereCollider->GetRadius();
		//	ray.dir = { 0.0f,-1.0f,0.0f,0.0f };
		//	RAYCAST_HIT raycastHit;

		//	// スムーズに坂を下る為の吸着距離
		//	const float adsDistance = 1.0f;

		//	//下の判定
		//	{
		//		// 接地を維持
		//		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
		//			float a = (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//			position.y -= a;
		//		}
		//	}
		//	//左の判定
		//	{
		//		ray.dir = { -1.0f,0.0f,0.0f,0.0f };
		//		// 接地を維持
		//		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		//		{
		//			float a = (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//			position.x -= a;
		//		}
		//	}
		//	//右の判定
		//	{
		//		ray.dir = { 1.0f,0.0f,0.0f,0.0f };
		//		// 接地を維持
		//		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		//		{
		//			float a = (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//			position.x += a;
		//		}
		//	}
		//	//前の判定
		//	{
		//		ray.dir = { 0.0f,0.0f,-1.0f,0.0f };
		//		// 接地を維持
		//		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		//		{
		//			float a = (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//			position.z -= a;
		//		}
		//	}
		//	//後の判定
		//	{
		//		ray.dir = { 0.0f,0.0f,1.0f,0.0f };
		//		// 接地を維持
		//		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		//		{
		//			float a = (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//			position.z += a;
		//		}
		//	}
	}

	{
		// 球の上端から球の下端までのレイキャスト
		Segment ray;
		ray.start = sphereCollider->center;
		ray.end = { pos.x + moveVec.x,pos.y + moveVec.y,pos.z + moveVec.z };
		Vector3 nMove = moveVec;
		nMove.normalize();
		ray.dir = { nMove.x, nMove.y,nMove.z,0.0f };
		RAYCAST_HIT raycastHit;

		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, moveVec.length())) {
			Vector3 a = { (raycastHit.inter.m128_f32[0] - pos.x) * 0.99f,
							(raycastHit.inter.m128_f32[1] - pos.y) * 0.99f,
							(raycastHit.inter.m128_f32[2] - pos.z) * 0.99f };
			if (abs(a.x) < 0.003f) { a.x = 0.0f; }
			if (abs(a.y) < 0.003f) { a.y = 0.0f; }
			if (abs(a.z) < 0.003f) { a.z = 0.0f; }

			pos += a;
		}
		else {
			pos += moveVec;
		}

		// 球の上端から球の下端までのレイキャスト
		ray.start = sphereCollider->center;
		ray.end = { pos.x,pos.y + moveVec.y,pos.z };
		nMove = moveVec;
		nMove.normalize();
		ray.dir = { 0,-1,0,0 };
		if (onGround) {
			// スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.2f;
			// 接地を維持
			if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
				onGround = true;
				pos.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
				jumpCount = 0; //連続ジャンプ回数をリセット
			}
			// 地面がないので落下
			else {
				onGround = false;
				fallV = {};
			}
		}
		// 落下状態
		else if (fallV.m128_f32[1] <= 0.0f) {
			if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
				// 着地
				onGround = true;
				pos.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
				jumpCount = 0; //連続ジャンプ回数をリセット
			}
		}

		moveVec = { 0.0f,0.0f,0.0f };

		object->SetPosition(pos);
	}

	//position.x += moveVec[2].x;
	//position.y += moveVec[2].y;
	//position.z += moveVec[2].z;
}
