#include "PlayerActionBlink.h"
#include "Player.h"
#include "Math/Easing/Easing.h"

PlayerActionBlink::PlayerActionBlink(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(blinkUseEndurance, 60, true); //持久力を使用

	//ブリンク回数を増やす(地面に着けばリセット)
	player->GetData()->blinkCount++;
	
	//タイマー生成
	blinkTimer = std::make_unique<Engine::Timer>();

	//落下速度を0にする
	player->GetData()->velocity.y = 0;
	player->GetData()->isNoGravity = true;

	//無敵状態
	player->GetData()->isNoDamage = true;

	//ブリンク方向を向くようにする
	const Vector3 moveRotaVelocity = { player->GetData()->avoidBlinkMoveVec.x, 0, player->GetData()->avoidBlinkMoveVec.z }; //プレイヤー回転にジャンプは関係ないので、速度Yは0にしておく
	player->SetMoveRotate(moveRotaVelocity, 15.0f);

	//回避アニメーションに変更
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ROLL_ANIMATION);

	//予め次の行動を設定しておく(終了後は通常移動)
	nextAction = PlayerActionName::MOVENORMAL;
}

PlayerActionBlink::~PlayerActionBlink()
{
}

void PlayerActionBlink::Update()
{
	//ブリンク
	Blink();
}

void PlayerActionBlink::Blink()
{
	//タイマー更新
	blinkTimer->Update();
	const float time = *blinkTimer.get() / (float)blinkTime;

	//イージングで速度を落としていく
	const float power = Easing::OutCirc(4, 1, time);
	player->GetData()->velocity = player->GetData()->avoidBlinkMoveVec.normalize() * power;

	//タイマーが指定した時間になったら行動変更先行入力
	if (*blinkTimer.get() >= actionChangeStartTime) {
		if (JumpStart()) { nextAction = PlayerActionName::JUMP; }
		if (AttackStart()) { nextAction = PlayerActionName::ATTACK; }
	}

	//タイマーが指定した時間になったらブリンク行動終了
	if (*blinkTimer.get() >= blinkTime) {
		isActionEnd = true;

		//予約していた次の行動をセット
		player->GetData()->action = nextAction;

		//無重力状態終了
		player->GetData()->isNoGravity = false;
		//無敵状態終了
		player->GetData()->isNoDamage = false;
	}
}
