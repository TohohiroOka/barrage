#include "Player.h"
#include "system/GameInputManager.h"
#include "GameHelper.h"
#include "Math/Vector2.h"
#include "game/camera/GameCamera.h"
#include "PlayerActionMoveNormal.h"
#include "PlayerActionJump.h"
#include "PlayerActionLightAttack.h"
#include "PlayerActionStrongAttack.h"
#include "PlayerActionAvoid.h"
#include "PlayerActionBlink.h"
#include "PlayerActionKnockback.h"
#include "PlayerActionDead.h"
#include "engine/Math/Easing/Easing.h"
#include "PlayerSwordAttack1.h"
#include <imgui.h>

using namespace DirectX;

const XMFLOAT3 Player::moveMinPos = { 0,0,0 };
const XMFLOAT3 Player::moveMaxPos = { GameHelper::Instance()->GetStageSize(),0.0f,GameHelper::Instance()->GetStageSize() };
float Player::gravityAccel = -0.01f;

Player::Player()
{
	model = FbxModel::Create("player");
	object = Fbx::Create(model.get());
	object->SetShadowMap(true);
	object->SetAnimation(true);
	object->SetIsBoneDraw(true);
	object->SetUseAnimation(PlayerAnimationName::JUMP_ANIMATION);

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
	object->SetBoneObject(bone, "rightHand", swordModel.get(), world, true, "sword1");

	//剣の長さ図り用
	{
		XMMATRIX matScale = XMMatrixScaling(0.0f, 0.0f, 0.0f);
		XMMATRIX matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
		matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
		matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));
		DirectX::XMMATRIX matTrans = XMMatrixTranslation(0.0f, 70.0f, 0.0f);
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		world *= matScale;
		world *= matRot;
		world *= matTrans;
		object->SetBoneObject(bone, "rightHand", swordModel.get(), world, false, "sword2");
	}
	object->SetScale({ 5,5,5 });

	//データ生成
	data = std::make_unique<PlayerData>();
	//行動生成
	ActionChange();

	//体力ゲージ
	hpGauge = std::make_unique<Gauge>(DirectX::XMFLOAT2({ 20.0f, 50.0f }), 600.0f, data->maxHP, data->HP, 2.0f, DirectX::XMFLOAT4({ 0.6f, 0.1f, 0.1f, 1.0f }));

	//持久力ゲージ
	enduranceGauge = std::make_unique<Gauge>(DirectX::XMFLOAT2({ 20.0f, 90.0f }), 600.0f, data->maxEndurance, data->endurance, 1.5f, DirectX::XMFLOAT4({ 0.1f, 0.6f, 0.1f, 1.0f }));

	//タイマー生成
	healTimer = std::make_unique<Engine::Timer>();
	enduranceRecoveryStartTimer = std::make_unique<Engine::Timer>();

	swordEffect = std::make_unique<AttachEmitter>("effect");
}

void Player::Update()
{
	//行動更新
	action->Update();
	if (action->GetIsActionEnd()) {
		ActionChange();
	}

	//待機モデルなしのための応急処置
	if (!(object->GetUseAnimation() == PlayerAnimationName::STAY_ANIMATION)) { object->SetAnimation(true); }

	//重力落下
	if (!data->isNoGravity) {
		Fall();
	}

	//進行方向に向きを変える
	MoveRotate();

	//更新した座標などを反映し、オブジェクト更新
	ObjectUpdate();

	//ゲージ更新
	HealHPMove();
	EnduranceRecovery();
	hpGauge->Update();
	enduranceGauge->Update();

	swordEffect->Add(object->GetAttachPos("sword1"), object->GetAttachPos("sword2"));
	swordEffect->Update();
}

void Player::Draw()
{
	swordEffect->Draw();
	object->Draw();

	if (!data->attackAction) { return; }
	data->attackAction->Draw();
}

void Player::DrawLightView()
{
	object->DrawLightView();
}

void Player::DrawSprite()
{
	hpGauge->Draw();
	enduranceGauge->Draw();
}

void Player::ImguiDraw()
{
	ImGui::SliderInt("maxHP", &data->maxHP, 0, 1000);

	ImGui::SliderInt("maxEndurance", &data->maxEndurance, 0, 1000);
	ImGui::SliderInt("dashUseEndurance", &PlayerData::dashUseEndurance, 0, 100);
	ImGui::SliderInt("avoidUseEndurance", &PlayerData::avoidUseEndurance, 0, 100);
	ImGui::SliderInt("jumpUseEndurance", &PlayerData::jumpUseEndurance, 0, 100);
	ImGui::SliderInt("blinkUseEndurance", &PlayerData::blinkUseEndurance, 0, 100);
	ImGui::SliderInt("swordAttackUseEnduranceNum", &PlayerSwordAttack1::attackUseEnduranceNum, 0, 100);

	ImGui::SliderInt("swordAttackPower", &PlayerSwordAttack1::attackPower, 0, 1000);

	ImGui::SliderFloat("moveSpeedMax", &PlayerActionMoveNormal::moveSpeedMax, 0.0f, 10.0f);
	ImGui::SliderFloat("dashSpeedMax", &PlayerActionMoveNormal::dashSpeedMax, 0.0f, 20.0f);

	ImGui::SliderInt("jumpMaxNum", &data->jumpMaxNum, 1, 5);
	ImGui::SliderFloat("playerFallAccel", &gravityAccel, 0.0f, -0.5f);
}

void Player::FrameReset()
{
	object->FrameReset();
}

void Player::Damage(int damageNum, const Vector3& knockbackVec, int knockbackPower, int knockbackTime, bool isKnockbackStart)
{
	//HPからダメージ量を引く
	data->HP -= damageNum;
	data->HP = max(data->HP, 0);

	hpGauge->ChangeLength(data->HP, true);

	//回復中なら回復を中断
	isHeal = false;

	//HPが0以下なら死亡
	if (data->HP <= 0) {
		data->action = PlayerActionName::DEAD;
		action = std::make_unique<PlayerActionDead>(this);
	}
	//ノックバックを開始するならノックバック状態にする
	else if (isKnockbackStart) {
		data->action = PlayerActionName::KNOCKBACK;
		action = std::make_unique<PlayerActionKnockback>(this, knockbackVec, knockbackTime, knockbackPower);
	}

}

void Player::Heal(int healNum)
{
	//回復前と回復後のHP量を計算
	healBeforeHP = data->HP;
	healAfterHP = data->HP + healNum;
	healAfterHP = min(healAfterHP, data->maxHP);

	//回復状態にする
	isHeal = true;
	healTimer = 0;
}

void Player::PushBack(const XMVECTOR& reject)
{
	//押し戻し
	Vector3 rejectNum = { reject.m128_f32[0],reject.m128_f32[1], reject.m128_f32[2] };
	data->pos += rejectNum;
}

void Player::SetMoveRotate(const Vector3& vec, float rotSpeed)
{
	//進行方向回転角を更新
	moveVelRota = VelocityRotate(vec);
	//横回転の回転角を0～360以内に収まるようにする
	Rotate360(moveVelRota.y);

	//回転スピードを設定
	moveRotSpeed = rotSpeed;
	//回転をなるべく短くするために元角度を調整(例：350→10 より -10→10の方が近い)
	if (moveVelRota.y > data->rota.y) {
		if (moveVelRota.y - data->rota.y > 180) {
			moveRotSpeed = -moveRotSpeed;
		}
	}
	else if (moveVelRota.y < data->rota.y) {
		if (data->rota.y - moveVelRota.y < 180) {
			moveRotSpeed = -moveRotSpeed;
		}
	}

	//移動回転を開始
	isMoveRotate = true;
}

void Player::UseEndurance(const int enduranceUseNum, const int enduranceRecoveryStartTime, bool isDecreaseDiffMode)
{
	//持久力を減らす
	data->endurance -= enduranceUseNum;
	data->endurance = max(data->endurance, 0);
	enduranceGauge->ChangeLength(data->endurance, isDecreaseDiffMode);

	//回復開始までにかかる時間をセット
	*enduranceRecoveryStartTimer.get() = enduranceRecoveryStartTime;
}

void Player::ObjectUpdate()
{
	//速度を加算して座標更新
	Vector3 modelMove = (Vector3)object->GetModelMove();
	if (object->GetUseAnimation() == PlayerAnimationName::STAY_ANIMATION) { modelMove = { 0,0,0 }; }
	data->pos += data->velocity * GameHelper::Instance()->GetGameSpeed() + modelMove;

	//壁判定
	data->pos.x = max(data->pos.x, moveMinPos.x + object->GetScale().x);
	data->pos.x = min(data->pos.x, moveMaxPos.x - object->GetScale().x);
	data->pos.z = max(data->pos.z, moveMinPos.z + object->GetScale().z);
	data->pos.z = min(data->pos.z, moveMaxPos.z - object->GetScale().z);

	//地面に接地判定
	const float modelHeight = 1; //スケール1のときのモデルの高さ
	if (data->pos.y <= object->GetScale().y * modelHeight + 0.5f) {
		data->pos.y = object->GetScale().y * modelHeight + 0.5f;
		data->velocity.y = 0;

		if (!data->onGround) {
			data->onGround = true;
			data->fallSpeed = 0;
			data->jumpCount = 0;
			data->blinkCount = 0; //連続ブリンク回数を0にする

			//ジャンプアニメーションなら待機アニメーションに変更(現在は待機アニメーションがないので代用中)
			if (object->GetUseAnimation() == PlayerAnimationName::JUMP_ANIMATION) {
				object->AnimationReset();
				object->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
			}
		}
	}
	//最終的な座標をセット
	object->SetPosition(data->pos);

	//オブジェクト更新
	object->Update();

	//待機モデルなしのための応急処置
	if (object->GetUseAnimation() == PlayerAnimationName::STAY_ANIMATION) { object->SetAnimation(false); }
}

void Player::ActionChange()
{
	//行動の変更が終わり、現在選択されている行動を開始する
	switch (data->action) {
	case PlayerActionName::MOVENORMAL:
		action = std::make_unique<PlayerActionMoveNormal>(this);
		break;

	case PlayerActionName::JUMP:
		action = std::make_unique<PlayerActionJump>(this);
		break;

	case PlayerActionName::LIGHTATTACK:
		action = std::make_unique<PlayerActionLightAttack>(this);
		break;

	case PlayerActionName::STRONGATTACK:
		action = std::make_unique<PlayerActionStrongAttack>(this);
		break;

	case PlayerActionName::AVOID:
		action = std::make_unique<PlayerActionAvoid>(this);
		break;

	case PlayerActionName::BLINK:
		action = std::make_unique<PlayerActionBlink>(this);
		break;

	default:
		assert(0);
		break;
	}
}

void Player::MoveRotate()
{
	//移動回転をしない場合は抜ける
	if (!isMoveRotate) { return; }

	//回転変化が微差ならそのまま進行方向を向く
	const float notRotDifference = 15.0f * GameHelper::Instance()->GetGameSpeed();
	if (fabsf(moveVelRota.y - data->rota.y) <= notRotDifference) {
		data->rota.y = moveVelRota.y;

		//微差を調整したら移動回転終了
		isMoveRotate = false;
	}
	else {
		//回転
		data->rota.y += moveRotSpeed * GameHelper::Instance()->GetGameSpeed();
		//横回転の回転角を0～360以内に収まるようにする
		Rotate360(data->rota.y);
	}

	//更新した回転角をセット
	object->SetRotation(data->rota);
}

void Player::Fall()
{
	float fallAcc = gravityAccel * GameHelper::Instance()->GetGameSpeed();

	//ジャンプ中で入力をし続けている場合は落下速度を減少させる
	if (data->jumpCount >= 1 && data->isInputJump && GameInputManager::PushInputAction(GameInputManager::Jump)) {
		fallAcc /= 3.5f;
	}
	else {
		data->isInputJump = false;
	}

	// 加速
	data->fallSpeed += fallAcc;
	data->velocity.y += data->fallSpeed;

	const float fallSppedMax = -10.0f;
	data->velocity.y = max(data->velocity.y, fallSppedMax);
}

void Player::HealHPMove()
{
	if (!isHeal) { return; }

	const float healTime = 10;
	healTimer->Update();
	const float time = *healTimer.get() / healTime;
	data->HP = (int)Easing::Lerp((float)healBeforeHP, (float)healAfterHP, time);
	hpGauge->ChangeLength(data->HP, false);

	if (*healTimer.get() >= healTime) {
		isHeal = false;
	}
}

void Player::EnduranceRecovery()
{
	//持久力が最大なら抜ける
	if (data->endurance >= data->maxEndurance) { return; }

	//タイマーが0になったら持久力を回復していく
	if (*enduranceRecoveryStartTimer.get() <= 0) {
		data->endurance++;
		data->endurance = min(data->endurance, data->maxEndurance);
		enduranceGauge->ChangeLength(data->endurance, false);
	}
	//それ以外ならタイマー更新
	else {
		*enduranceRecoveryStartTimer.get() -= 1;
	}
}
