#include "PlayerActionAvoid.h"
#include "player.h"
#include "Math/Easing/Easing.h"

PlayerActionAvoid::PlayerActionAvoid(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(avoidUseEndurance, 45, true); //持久力を使用

	//タイマー生成
	avoidTimer = std::make_unique<Engine::Timer>();

	//無敵状態
	player->GetData()->isNoDamage = true;

	//回避方向を向くようにする
	const Vector3 moveRotaVelocity = { player->GetData()->avoidBlinkMoveVec.x, 0, player->GetData()->avoidBlinkMoveVec.z }; //プレイヤー回転にジャンプは関係ないので、速度Yは0にしておく
	player->SetMoveRotate(moveRotaVelocity, 15.0f);

	//回避アニメーションに変更
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ROLL_ANIMATION);

	//予め次の行動を設定しておく(終了後は通常移動)
	nextAction = PlayerActionName::MOVENORMAL;
}

PlayerActionAvoid::~PlayerActionAvoid()
{
}

void PlayerActionAvoid::Update()
{
	//回避
	Avoid();
}

void PlayerActionAvoid::Avoid()
{
	//タイマー更新
	avoidTimer->Update();
	const float time = *avoidTimer.get() / (float)avoidTime;

	//イージングで速度を落としていく
	const float power = Easing::OutCirc(2, 0.5f, time);
	player->GetData()->velocity = player->GetData()->avoidBlinkMoveVec.normalize() * power;


	//タイマーが指定した時間になったら行動変更先行入力
	if (*avoidTimer.get() >= actionChangeStartTime) {
		if (JumpStart()) { nextAction = PlayerActionName::JUMP; }
		if (AttackStart()) { nextAction = PlayerActionName::ATTACK; }
	}


	//タイマーが指定した時間になったら回避行動終了
	if (*avoidTimer.get() >= avoidTime) {
		isActionEnd = true;

		//予約していた次の行動をセット
		player->GetData()->action = nextAction;

		//無敵状態終了
		player->GetData()->isNoDamage = false;
	}
}
