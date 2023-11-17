#include "Player.h"
#include "system/GameInputManager.h"
#include "GameHelper.h"
#include "Object/3d/collider/SphereCollider.h"
#include "Object/3d/collider/CollisionManager.h"
#include "Object/3d/collider/CollisionAttribute.h"
#include "Math/Vector2.h"
#include "game/camera/GameCamera.h"
#include "engine/Math/Easing/Easing.h"
#include "PlayerSwordAttack1.h"
#include <imgui.h>

using namespace DirectX;

const XMFLOAT3 Player::moveMinPos = { 0,0,0 };
const XMFLOAT3 Player::moveMaxPos = { 510,0,510 };
float Player::jumpPower = 3.0f;
float Player::gravityAccel = -0.015f;
const float Player::moveSpeedMax = 1.25f;
const float Player::dashSpeedMax = 2.5f;

Player::Player()
{
	model = FbxModel::Create("player");
	object = Fbx::Create(model.get());
	object->SetShadowMap(true);
	object->SetAnimation(true);
	object->SetIsBoneDraw(true);
	object->SetUseAnimation(AnimationName::JUMP);

	//剣モデル読み込み
	swordModel = Model::CreateFromOBJ("sword");
	std::string bone = "mixamorig:RightHand";
	XMMATRIX matScale = XMMatrixScaling(50.0f, 50.0f, 50.0f);
	XMMATRIX matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
	matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));
	DirectX::XMMATRIX matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world *= matScale;
	world *= matRot;
	world *= matTrans;
	object->SetBoneObject(bone, "rightHand", swordModel.get(), world);

	pos = { 100.0f,50.0f,100.0f };
	object->SetScale({ 5,5,5 });
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

	//タイマークラス
	avoidTimer = std::make_unique<Engine::Timer>();
	blinkTimer = std::make_unique<Engine::Timer>();
	healTimer = std::make_unique<Engine::Timer>();
	enduranceRecoveryStartTimer = std::make_unique<Engine::Timer>();
}

void Player::Update()
{
	//毎フレーム戻しておく
	isMoveKey = false;
	isMovePad = false;

	if (isKnockback) {
		Knockback();
	}
	else if (isBlink) {
		Blink();
	}
	else if (isAvoid) {
		Avoid();
	}
	else {
		Move();
		Jump();
		Attack();
		AvoidStart();
		BlinkStart();
	}

	if (!isBlink) {
		Fall();
	}

	//進行方向に向きを変える
	MoveRotate();

	HealHPMove();
	EnduranceRecovery();

	//更新した座標などを反映し、オブジェクト更新
	ObjectUpdate();

	hpGauge->Update();
	enduranceGauge->Update();

	if (attackAction) {
		attackAction->Update();
		//攻撃の行動が終了したら解放
		if (attackAction->GetIsAttackActionEnd()) {
			attackAction.release();
			isAttack = false;

			//攻撃アニメーションなら待機アニメーションに変更(現在は待機アニメーションがないので代用中)
			if (object->GetUseAnimation() == AnimationName::ATTACK_RIGHT) {
				object->AnimationReset();
				object->SetUseAnimation(AnimationName::ATTACK_LEFT);
			}
		}
	}
}

void Player::Draw()
{
	object->Draw();

	if (attackAction) {
		attackAction->Draw();
	}
}

void Player::DrawSprite()
{
	hpGauge->Draw();
	enduranceGauge->Draw();
}

void Player::FrameReset()
{
	object->FrameReset();
}

void Player::ImguiDraw()
{
	ImGui::SliderFloat("playerFallAccel", &gravityAccel, 0.0f, -0.5f);
	ImGui::SliderFloat("playerJumpPower", &jumpPower, 0.0f, 20.0f);
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
	//ブリンク中なら中断
	isBlink = false;

	//ノックバック状態にする
	KnockbackStart(subjectPos, damageNum);

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

void Player::PushBack(const XMVECTOR& reject)
{
	//押し戻し
	Vector3 rejectNum = { reject.m128_f32[0],reject.m128_f32[1], reject.m128_f32[2] };
	pos += rejectNum;
}

void Player::ObjectUpdate()
{
	//速度を加算して座標更新
	pos += velocity * GameHelper::Instance()->GetGameSpeed();

	//壁判定
	pos.x = max(pos.x, moveMinPos.x);
	pos.x = min(pos.x, moveMaxPos.x);
	pos.z = max(pos.z, moveMinPos.z);
	pos.z = min(pos.z, moveMaxPos.z);

	//地面に接地判定
	const float modelHeight = 1; //スケール1のときのモデルの高さ
	if (pos.y <= object->GetScale().y * modelHeight + 0.5f) {
		pos.y = object->GetScale().y * modelHeight + 0.5f;
		if (!onGround) {
			onGround = true;
			fallSpeed = 0;
			jumpCount = 0;
			isBlinkStart = true; //ブリンク開始可能にする

			//ジャンプアニメーションなら待機アニメーションに変更(現在は待機アニメーションがないので代用中)
			if (object->GetUseAnimation() == AnimationName::JUMP) {
				object->AnimationReset();
				object->SetUseAnimation(AnimationName::ATTACK_LEFT);
			}
		}
	}
	//最終的な座標をセット
	object->SetPosition(pos);

	//オブジェクト更新
	object->Update();
}

void Player::Move()
{
	DirectInput* input = DirectInput::GetInstance();

	//移動キー入力を判定
	isMoveKey = (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveRight).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveLeft).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveForward).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveBack).key));

	//ある程度スティックを傾けないと移動パッド入力判定しない
	const XMFLOAT2 padIncline = GameInputManager::GetPadLStickIncline();
	isMovePad = (fabsf(padIncline.x) >= GameInputManager::GetPadStickInputIncline() || fabsf(padIncline.y) >= GameInputManager::GetPadStickInputIncline());

	//ダッシュ
	Dash();

	//入力
	const float moveAccel = 0.1f * GameHelper::Instance()->GetGameSpeed();
	if (isMoveKey || isMovePad) {
		moveSpeed += moveAccel;
		if (isDash) { moveSpeed = min(moveSpeed, dashSpeedMax); }
		else { moveSpeed = min(moveSpeed, moveSpeedMax); }

		Vector3 inputMoveVec{};

		if (isMoveKey) {
			if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveRight).key)) {
				inputMoveVec.x = 1;
			}
			if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveLeft).key)) {
				inputMoveVec.x = -1;
			}
			if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveForward).key)) {
				inputMoveVec.z = 1;
			}
			if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveBack).key)) {
				inputMoveVec.z = -1;
			}
		}
		if (isMovePad) {
			//パッドスティックの方向をベクトル化
			inputMoveVec.x = GameInputManager::GetPadLStickIncline().x;
			inputMoveVec.z = GameInputManager::GetPadLStickIncline().y;
		}

		//ベクトルをカメラの傾きで回転させる
		inputMoveVec.normalize();
		const float cameraRotaRadian = XMConvertToRadians(-gameCamera->GetCameraRota().y);
		moveVec.x = inputMoveVec.x * cosf(cameraRotaRadian) - inputMoveVec.z * sinf(cameraRotaRadian);
		moveVec.z = inputMoveVec.x * sinf(cameraRotaRadian) + inputMoveVec.z * cosf(cameraRotaRadian);

		//進行方向を向くようにする
		SetMoveRotate();

		//待機アニメーションのときのみ走るアニメーションを開始する(現在は待機アニメーションがないので代用中)
		if (object->GetUseAnimation() == AnimationName::ATTACK_LEFT) {
			object->AnimationReset();
			object->SetUseAnimation(AnimationName::RUN);
		}
	}
	else {
		moveSpeed -= moveAccel;
		moveSpeed = max(moveSpeed, 0);

		//スピードがなくなったら待機アニメーションに変更(設計中)
		if (object->GetUseAnimation() == AnimationName::RUN && moveSpeed <= 0) {
			object->AnimationReset();
			object->SetUseAnimation(AnimationName::ATTACK_LEFT);
		}
	}

	//速度をセット
	velocity.x = moveVec.x * moveSpeed;
	velocity.z = moveVec.z * moveSpeed;
}

void Player::SetMoveRotate()
{
	//進行方向回転角を更新
	const Vector3 moveRotaVelocity = { moveVec.x, 0, moveVec.z };//プレイヤー回転にジャンプは関係ないので、速度Yは0にしておく
	moveVelRota = VelocityRotate(moveRotaVelocity);
	//横回転の回転角を0～360以内に収まるようにする
	Rotate360(moveVelRota.y);

	//回転スピードを設定
	moveRotSpeed = 15.0f;
	//回転をなるべく短くするために元角度を調整(例：350→10 より -10→10の方が近い)
	if (moveVelRota.y > rota.y) {
		if (moveVelRota.y - rota.y > 180) {
			moveRotSpeed = -moveRotSpeed;
		}
	}
	else if (moveVelRota.y < rota.y) {
		if (rota.y - moveVelRota.y < 180) {
			moveRotSpeed = -moveRotSpeed;
		}
	}

	//移動回転を開始
	isMoveRotate = true;
}

void Player::MoveRotate()
{
	//移動回転をしない場合は抜ける
	if (!isMoveRotate) { return; }

	//回転変化が微差ならそのまま進行方向を向く
	const float notRotDifference = 15.0f * GameHelper::Instance()->GetGameSpeed();
	if (fabsf(moveVelRota.y - rota.y) <= notRotDifference) {
		rota.y = moveVelRota.y;

		//微差を調整したら移動回転終了
		isMoveRotate = false;
	}
	else {
		//回転
		rota.y += moveRotSpeed * GameHelper::Instance()->GetGameSpeed();
		//横回転の回転角を0～360以内に収まるようにする
		Rotate360(rota.y);
	}

	//更新した回転角をセット
	object->SetRotation(rota);
}

void Player::Dash()
{
	//地面にいない場合は、変更を受け付けないで抜ける
	if (!onGround) { return; }

	if (!isDash) {
		//ダッシュ開始可能で地面接地しているかつ、ダッシュ入力があって移動した場合にダッシュ状態にする
		if (isDashStart && (isMoveKey || isMovePad) && endurance > 0 && GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash)) {
			isDash = true;
			isDashStart = false;
		}

		//ダッシュ開始不能時は、ダッシュボタン入力を一度離すことで可能になる
		if ((!isDashStart) && (!GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash))) {
			isDashStart = true;
		}
	}
	else {
		//移動 & 入力中はダッシュ状態を維持
		if ((isMoveKey || isMovePad) && endurance > 0 && GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash)) {
			UseEndurance(dashUseEndurance, 1, false); //持久力を使用
		}
		//入力が途切れたときにダッシュを終了する
		else {
			isDash = false;
		}
	}
}

void Player::Fall()
{
	//地面に接地していたら抜ける
	//if (onGround) { return; }

	float fallAcc = gravityAccel * GameHelper::Instance()->GetGameSpeed();

	//ジャンプ中で入力をし続けている場合は落下速度を減少させる
	if (jumpCount >= 1 && isInputJump && GameInputManager::PushInputAction(GameInputManager::Jump)) {
		fallAcc /= 3.5f;
	}
	else {
		isInputJump = false;
	}

	// 加速
	fallSpeed += fallAcc;
	velocity.y += fallSpeed;

	const float fallSppedMax = -10.0f;
	velocity.y = max(velocity.y, fallSppedMax);
}

void Player::AvoidStart()
{
	//地面にいない場合は抜ける
	if (!onGround) { return; }
	//移動中に回避入力がなければ抜ける
	if (!((isMoveKey || isMovePad) && GameInputManager::TriggerInputAction(GameInputManager::Avoid_Blink_Dash))) { return; }
	//持久力が回避で使用する値以下なら抜ける	
	if (endurance < avoidUseEndurance) { return; }
	UseEndurance(avoidUseEndurance, 30, true); //持久力を使用

	avoidTimer->Reset();
	isAvoid = true;
	isDash = false;
	isDashStart = true;

	object->AnimationReset();
	object->SetUseAnimation(AnimationName::ROLL);
}

void Player::Avoid()
{
	//タイマー更新
	const float avoidTime = 60;
	avoidTimer->Update();
	const float time = *avoidTimer.get() / avoidTime;

	const float power = Easing::OutCirc(8, 1, time);

	velocity = moveVec.normalize() * power;

	//タイマーが指定した時間になったら回避終了
	if (*avoidTimer.get() >= avoidTime) {
		isAvoid = false;

		//待機アニメーションに変更(設計中)
		object->AnimationReset();
		object->SetUseAnimation(AnimationName::ATTACK_LEFT);
	}
}

void Player::Jump()
{
	//ジャンプ回数が連続ジャンプ可能回数を超えていたら抜ける
	if (jumpCount >= jumpMaxNum) { return; }
	//ジャンプ入力がなければ抜ける
	if (!GameInputManager::TriggerInputAction(GameInputManager::Jump)) { return; }
	//持久力がジャンプで使用する値以下なら抜ける
	if (endurance < jumpUseEndurance) { return; }
	UseEndurance(jumpUseEndurance, 30, true); //持久力を使用

	onGround = false;
	isInputJump = true;
	velocity.y = jumpPower;
	fallSpeed = 0;
	jumpCount++; //ジャンプ回数を増やす

	object->AnimationReset();
	object->SetUseAnimation(AnimationName::JUMP);
}

void Player::BlinkStart()
{
	//ブリンク開始可能でなければ抜ける
	if (!isBlinkStart) { return; }
	//ジャンプ中でなければ抜ける
	if (!(jumpCount >= 1)) { return; }
	//ブリンク入力がなければ抜ける
	if (!GameInputManager::TriggerInputAction(GameInputManager::Avoid_Blink_Dash)) { return; }
	//持久力がブリンクで使用する値以下なら抜ける	
	if (endurance < blinkUseEndurance) { return; }
	UseEndurance(blinkUseEndurance, 30, true); //持久力を使用

	//落下速度を0にする
	velocity.y = 0;

	blinkTimer->Reset();
	isBlink = true;
	isBlinkStart = false;

	object->AnimationReset();
	object->SetUseAnimation(AnimationName::ROLL);
}

void Player::Blink()
{
	//タイマー更新
	const float blinkTime = 90;
	blinkTimer->Update();
	const float time = *blinkTimer.get() / blinkTime;

	const float power = Easing::OutCirc(8, 1, time);

	velocity = moveVec.normalize() * power;

	//タイマーが指定した時間になったらブリンク終了
	if (*blinkTimer.get() >= blinkTime) {
		isBlink = false;

		//待機アニメーションに変更(設計中)
		object->AnimationReset();
		object->SetUseAnimation(AnimationName::ATTACK_LEFT);
	}
}

void Player::Attack()
{
	if (!isAttack) {
		//入力で攻撃をセット
		if (GameInputManager::TriggerInputAction(GameInputManager::Attack)) {
			attackAction = std::make_unique<PlayerSwordAttack1>(object.get());
			if (!attackAction->NextAttack(endurance)) { return; }

			UseEndurance(attackAction->GetUseEndranceNum(), 30, true);
			isAttack = true;

			object->AnimationReset();
			object->SetUseAnimation(AnimationName::ATTACK_RIGHT);
		}
	}
	else {
		//次の攻撃を入力可能なら
		if (attackAction->GetIsNextAttackInput()) {
			//入力で攻撃をセット
			if (GameInputManager::TriggerInputAction(GameInputManager::Attack)) {
				if (!attackAction->NextAttack(endurance)) { return; }

				UseEndurance(attackAction->GetUseEndranceNum(), 30, true);

				object->AnimationReset();
				object->SetUseAnimation(AnimationName::ATTACK_RIGHT);
			}
		}
	}
}

void Player::HealHPMove()
{
	if (!isHeal) { return; }

	const float healTime = 10;
	healTimer->Update();
	const float time = *healTimer.get() / healTime;
	HP = (int)Easing::Lerp((float)healBeforeHP, (float)healAfterHP, time);
	hpGauge->ChangeLength(HP, false);

	if (*healTimer.get() >= healTime) {
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
	*enduranceRecoveryStartTimer.get() = enduranceRecoveryStartTime;
}

void Player::EnduranceRecovery()
{
	//持久力が最大なら抜ける
	if (endurance >= maxEndurance) { return; }


	//タイマーが0になったら持久力を回復していく
	if (*enduranceRecoveryStartTimer.get() <= 0) {
		endurance++;
		endurance = min(endurance, maxEndurance);
		enduranceGauge->ChangeLength(endurance, false);
	}
	//それ以外ならタイマー更新
	else {
		*enduranceRecoveryStartTimer.get() -= 1;
	}
}

void Player::KnockbackStart(const Vector3& subjectPos, int power)
{
	//攻撃対象と自分のベクトルを算出
	knockbackVec = pos - subjectPos;

	//ノックバックの強さをセット(仮で食らったダメージ量 / 10)
	knockbackPower = (float)power / 10;

	//移動スピードを0にしておく
	moveSpeed = 0;

	isKnockback = true;
}

void Player::Knockback()
{
	//ノックバック
	velocity = knockbackVec.normalize() * knockbackPower;

	//ノックバックを弱くしていく
	knockbackPower -= 0.05f * GameHelper::Instance()->GetGameSpeed();

	//ノックバックによる移動がなくなったらノックバック状態終了
	if (knockbackPower < 0) {
		isKnockback = false;
	}
}
