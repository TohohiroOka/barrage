#include "PlayerActionAvoid.h"
#include "player.h"
#include "system/GameInputManager.h"
#include "Math/Easing/Easing.h"
#include "engine/Audio/Audio.h"

const float PlayerActionAvoid::avoidStartMoveSpeed = 1.1f;
const float PlayerActionAvoid::avoidEndMoveSpeed = 0.4f;

PlayerActionAvoid::PlayerActionAvoid(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(PlayerData::avoidUseEndurance, 45, true); //持久力を使用

	//タイマー生成
	avoidTimer = std::make_unique<Engine::Timer>();

	//無敵状態
	player->GetData()->isNoDamage = true;

	//回避方向を向くようにする
	Vector3 moveRotaVelocity = player->GetData()->avoidBlinkMoveVec;
	moveRotaVelocity.y = 0; //プレイヤー回転にジャンプは関係ないので、速度Yは0にしておく
	player->SetMoveRotate(moveRotaVelocity, 15.0f);

	//回避アニメーションに変更
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ROLL_ANIMATION);

	//予め次の行動を設定しておく(終了後は通常移動)
	nextAction = PlayerActionName::MOVENORMAL;

	//回避音再生
	Audio::Instance()->SoundPlayWava(Sound::SoundName::avoid, false, 0.1f);
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

	//イージングで移動スピードを落としていく
	player->GetData()->moveSpeed = Easing::InCirc(avoidStartMoveSpeed, avoidEndMoveSpeed, time);
	player->GetData()->velocity = player->GetData()->avoidBlinkMoveVec.normalize() * player->GetData()->moveSpeed;


	//タイマーが指定した時間になったら行動変更先行入力
	if (*avoidTimer.get() >= actionChangeStartTime) {
		if (JumpStart()) { nextAction = PlayerActionName::JUMP; }
		else if (GameInputManager::TriggerInputAction(GameInputManager::LightAttack)) { nextAction = PlayerActionName::LIGHTATTACK; }
		else if (GameInputManager::TriggerInputAction(GameInputManager::StrongAttack)) { nextAction = PlayerActionName::STRONGATTACK; }
	}


	//タイマーが指定した時間になったら回避行動終了
	if (*avoidTimer.get() >= avoidTime) {
		isActionEnd = true;

		//先行入力で弱攻撃または強攻撃を選択している場合は最終チェック
		if (nextAction == PlayerActionName::LIGHTATTACK || nextAction == PlayerActionName::STRONGATTACK) {
			bool isNextActionAttack = false;
			if (nextAction == PlayerActionName::LIGHTATTACK && LightAttackStart()) { isNextActionAttack = true; }
			else if (nextAction == PlayerActionName::STRONGATTACK && StrongAttackStart()) { isNextActionAttack = true; }

			if (!isNextActionAttack) { nextAction = PlayerActionName::MOVENORMAL; }
		}

		//予約していた次の行動をセット
		player->GetData()->action = nextAction;

		//無敵状態終了
		player->GetData()->isNoDamage = false;
	}
}
