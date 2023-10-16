#include "Player.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "GameHelper.h"
#include "Object/3d/collider/SphereCollider.h"
#include "Object/3d/collider/CollisionManager.h"
#include "Object/3d/collider/CollisionAttribute.h"
#include "Math/Vector2.h"
#include "game/camera/GameCamera.h"
#include "engine/Math/Easing/Easing.h"
#include "PlayerSwordAttack1.h"

using namespace DirectX;

const float Player::moveSpeedMax = 1.5f;
const float Player::dashSpeedMax = 2.0f;

Player::Player()
{
	model = Model::CreateFromOBJ("NormalCube");
	object = Object3d::Create(model.get());
	pos = { 100.0f,200.0f,100.0f };
	moveVec = { 0.0f,0.0f,0.0f };

	// コライダーの追加
	float radius = 0.6f;
	object->SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	object->GetCollider()->SetAttribute(COLLISION_ATTR_ALLIES);
	object->SetShadowMap(true);
	//連続ジャンプ可能回数設定
	jumpMaxNum = 2;

	//最大体力をセット
	maxHP = 100;
	HP = maxHP;
	hpGauge = std::make_unique<Gauge>(DirectX::XMFLOAT2({ 20.0f, 50.0f }), 600.0f, maxHP, HP, DirectX::XMFLOAT4({ 0.6f, 0.1f, 0.1f, 1.0f }));

	//最大持久力をセット
	maxEndurance = 200;
	endurance = maxEndurance;
	enduranceGauge = std::make_unique<Gauge>(DirectX::XMFLOAT2({ 20.0f, 90.0f }), 600.0f, maxEndurance, endurance, DirectX::XMFLOAT4({ 0.1f, 0.6f, 0.1f, 1.0f }));
}

void Player::Update()
{
	DirectInput* input = DirectInput::GetInstance();
	if (input->TriggerKey(DIK_1)) {
		Damage(20, {});
	}
	if (input->PushKey(DIK_2)) {
		endurance--;
		endurance = max(endurance, 0);
		enduranceGauge->ChangeLength(endurance, false);
	}
	if (input->TriggerKey(DIK_3)) {
		endurance -= 20;
		endurance = max(endurance, 0);
		enduranceGauge->ChangeLength(endurance, true);
	}
	if (input->TriggerKey(DIK_4)) {
		Heal(30);
	}

	if (isKnockback) {
		Knockback();
	}
	else {
		Move();

		Jump();

		Attack();

		HealHPMove();
		EnduranceRecovery();
	}
	if (!onGround) {
		// 落下処理
		Fall();
	}
	Collider();

	object->Update();

	hpGauge->Update();
	enduranceGauge->Update();

	if (attackAction) {
		attackAction->Update();
		//攻撃の行動が終了したら解放
		if (attackAction->GetIsAttackActionEnd()) {
			attackAction.release();
			isAttack = false;
		}
	}
}

void Player::Draw()
{
	object->Draw();

	hpGauge->Draw();
	enduranceGauge->Draw();

	if (attackAction) {
		attackAction->Draw();
	}
}

void Player::DrawLightView()
{
	object->DrawLightView();

	if (attackAction) {
		attackAction->DrawLightView();
	}
}

void Player::Damage(int damageNum, const Vector3& subjectPos)
{
	//HPからダメージ量を引く
	HP -= damageNum;
	HP = max(HP, 0);

	hpGauge->ChangeLength(HP, true);

	//回復中なら回復を中断
	isHeal = false;

	//ノックバック状態にする
	SetKnockback(subjectPos, damageNum);

	//HPが0以下なら死亡
	if (!(HP <= 0)) { return; }

	isDead = true;
}

void Player::Heal(int healNum)
{
	//回復前と回復後のHP量を計算
	healBeforeHP = HP;
	healAfterHP = HP + healNum;
	healAfterHP = min(healAfterHP, maxHP);

	//回復状態にする
	isHeal = true;
	healTimer = 0;
}

void Player::Move()
{
	DirectInput* input = DirectInput::GetInstance();

	//ダッシュ
	Dash();

	//キー入力
	{
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_W) || input->PushKey(DIK_S)) {

			Vector3 moveKeyVec{};
			if (input->PushKey(DIK_D)) {
				moveKeyVec.x += moveSpeed;
			}
			if (input->PushKey(DIK_A)) {
				moveKeyVec.x -= moveSpeed;
			}
			if (input->PushKey(DIK_W)) {
				moveKeyVec.z += moveSpeed;
			}
			if (input->PushKey(DIK_S)) {
				moveKeyVec.z -= moveSpeed;
			}

			//キー入力のベクトルをカメラの傾きで回転させる
			const float cameraRotaRadian = XMConvertToRadians(-gameCamera->GetCameraRota().y);
			Vector3 cameraVec{};
			cameraVec.x = moveKeyVec.x * cosf(cameraRotaRadian) - moveKeyVec.z * sinf(cameraRotaRadian);
			cameraVec.z = moveKeyVec.x * sinf(cameraRotaRadian) + moveKeyVec.z * cosf(cameraRotaRadian);

			moveVec = cameraVec;

			//進行方向を向くようにする
			Vector3 moveRotaVelocity = { moveVec.x, 0, moveVec.z };//プレイヤー回転にジャンプは関係ないので、速度Yは0にしておく
			rota = GameHelper::Instance()->VelocityRotate(moveRotaVelocity);
			object->SetRotation(rota);
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

		//パッドスティックの方向をベクトル化
		Vector3 moveStickVec{};
		moveStickVec.x = XInputManager::GetInstance()->GetPadLStickIncline().x * moveSpeed;
		moveStickVec.z = XInputManager::GetInstance()->GetPadLStickIncline().y * moveSpeed;

		//パッドスティックのベクトルをカメラの傾きで回転させる
		const float cameraRotaRadian = XMConvertToRadians(-gameCamera->GetCameraRota().y);
		Vector3 cameraVec{};
		cameraVec.x = moveStickVec.x * cosf(cameraRotaRadian) - moveStickVec.z * sinf(cameraRotaRadian);
		cameraVec.z = moveStickVec.x * sinf(cameraRotaRadian) + moveStickVec.z * cosf(cameraRotaRadian);

		moveVec = cameraVec;

		//進行方向を向くようにする
		Vector3 moveRotaVelocity = { moveVec.x, 0, moveVec.z };//プレイヤー回転にジャンプは関係ないので、速度Yは0にしておく
		rota = GameHelper::Instance()->VelocityRotate(moveRotaVelocity);
		object->SetRotation(rota);
	}
}

void Player::Dash()
{
	//ダッシュ入力があった場合に、移動スピードを速くしていく
	if ((DirectInput::GetInstance()->PushKey(DIK_Z) || XInputManager::GetInstance()->PushButton(XInputManager::PAD_B)) && endurance > 0) {
		moveSpeed = dashSpeedMax;
		UseEndurance(1, 1, false); //持久力を使用
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

	//持久力がジャンプで使用する値以下なら抜ける
	const int jumpUseEndurance = 30;
	if (endurance < jumpUseEndurance) { return; }
	UseEndurance(jumpUseEndurance, 30, true); //持久力を使用

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
			XMFLOAT3 hugou = {};
			if (abs(moveVec.x) < 0.0001f) { hugou.x = 0.0f; }
			else { hugou.x = adsDistance * moveVec.x / abs(moveVec.x); }
			if (abs(moveVec.y) < 0.0001f) { hugou.y = 0.0f; }
			else { hugou.y = adsDistance * moveVec.y / abs(moveVec.y); }
			if (abs(moveVec.z) < 0.0001f) { hugou.z = 0.0f; }
			else { hugou.z = adsDistance * moveVec.z / abs(moveVec.z); }
			Vector3 a = { (raycastHit.inter.m128_f32[0] - pos.x - hugou.x),
							(raycastHit.inter.m128_f32[1] - pos.y - hugou.y),
							(raycastHit.inter.m128_f32[2] - pos.z - hugou.z) };
			if (abs(a.x) < 0.003f) { a.x = 0.0f; }
			if (abs(a.z) < 0.003f) { a.z = 0.0f; }
			if (abs(a.y) < 0.003f || returnY) {
				a.y = 0.0f;
				onGround = true;
			}

			returnY = true;
			pos += a;
		}
		else {
			pos += moveVec;
			returnY = false;
		}

		// 球の上端から球の下端までのレイキャスト
		ray.start = sphereCollider->center;
		ray.end = { pos.x,pos.y + moveVec.y,pos.z };
		nMove = moveVec;
		nMove.normalize();
		ray.dir = { 0,-1,0,0 };
		if (onGround) {
			// 接地を維持
			if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
				onGround = true;
				returnY = false;
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
				returnY = false;
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

void Player::Attack()
{
	if (!isAttack) {
		//入力で攻撃をセット
		if (DirectInput::GetInstance()->TriggerKey(DIK_Q) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_RB)) {
			attackAction = std::make_unique<PlayerSwordAttack1>(object.get());
			if (!attackAction->NextAttack(endurance)) { return; }

			UseEndurance(attackAction->GetUseEndranceNum(), 30, true);
			isAttack = true;
		}
	}
	else {
		//次の攻撃を入力可能なら
		if (attackAction->GetIsNextAttackInput()) {
			//入力で攻撃をセット
			if (DirectInput::GetInstance()->TriggerKey(DIK_Q) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_RB)) {
				if (!attackAction->NextAttack(endurance)) { return; }

				UseEndurance(attackAction->GetUseEndranceNum(), 30, true);
			}
		}
	}
}

void Player::HealHPMove()
{
	if (!isHeal) { return; }

	const float healTime = 10;
	healTimer++;
	const float time = healTimer / healTime;
	HP = (int)Easing::Lerp((float)healBeforeHP, (float)healAfterHP, time);
	hpGauge->ChangeLength(HP, false);

	if (healTimer >= healTime) {
		isHeal = false;
	}
}

void Player::UseEndurance(const int enduranceUseNum, const int enduranceRecoveryStartTime, bool isDecreaseDiffMode)
{
	//持久力を減らす
	endurance -= enduranceUseNum;
	endurance = max(endurance, 0);
	enduranceGauge->ChangeLength(endurance, isDecreaseDiffMode);

	//回復開始までにかかる時間をセット
	enduranceRecoveryStartTimer = enduranceRecoveryStartTime;
}

void Player::EnduranceRecovery()
{
	//持久力が最大なら抜ける
	if (endurance >= maxEndurance) { return; }


	//タイマーが0になったら持久力を回復していく
	if (enduranceRecoveryStartTimer <= 0) {
		endurance++;
		endurance = min(endurance, maxEndurance);
		enduranceGauge->ChangeLength(endurance, false);
	}
	//それ以外ならタイマー更新
	else {
		enduranceRecoveryStartTimer--;
	}
}

void Player::SetKnockback(const Vector3& subjectPos, int power)
{
	//攻撃対象と自分のベクトルを算出
	knockbackVec = pos - subjectPos;
	knockbackVec.y = 0;

	//ノックバックの強さをセット(仮で食らったダメージ量 / 10)
	knockbackPower = (float)power / 10;

	isKnockback = true;
}

void Player::Knockback()
{
	//ノックバック
	moveVec = knockbackVec.normalize() * knockbackPower;
	
	//ノックバックを弱くしていく
	knockbackPower -= 0.05f;

	//ノックバックによる移動がなくなったらノックバック状態終了
	if (knockbackPower < 0) {
		isKnockback = false;
	}
}
