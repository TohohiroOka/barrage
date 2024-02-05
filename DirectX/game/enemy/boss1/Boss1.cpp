#include "Boss1.h"
#include "GameHelper.h"

#include "Boss1Model.h"
#include "../game/enemy/boss1/attack/Boss1NearAttack1.h"
#include "../game/enemy/boss1/move/Boss1Move1.h"
#include "../game/enemy/boss1/move/Boss1Move2.h"
#include "../game/enemy/boss1/move/Boss1Move3.h"
#include "../game/enemy/boss1/attack/Boss1Bullet1.h"
#include "../game/enemy/boss1/attack/Boss1Bullet2.h"
#include "../game/enemy/boss1/attack/Boss1Bullet3.h"
#include "../game/enemy/boss1/attack/Boss1Bullet4.h"
#include "../game/enemy/boss1/attack/Boss1HalfAttack.h"
#include "../game/enemy/boss1/attack/ScreenBreakAttack.h"

#include "Boss1Wince.h"

Boss1::Boss1()
{
	maxHP = 2000;

	BaseBoss::Initialize();

	bossModel = std::make_unique<Boss1Model>();

	BaseAction::SetBossPtr(this);

	action = std::make_unique<Boss1Move1>();

	hitScale = bossModel->GetObjectInst()->GetScale().y * 5000.0f;

	breakHp[0] = maxHP * 0.7f;
	breakHp[1] = maxHP * 0.3f;
}

void Boss1::Update()
{
	if (HP < breakHp[0] && breakNum==0) {
		action->SetActionBreak();
		breakAction = std::make_unique<ScreenBreakAttack>();
		isBreak = true;
		breakNum++;
	} else if (HP < breakHp[1] && breakNum == 1) {
		action->SetActionBreak();
		breakAction = std::make_unique<Boss1HalfAttack>();
		isBreak = true;
		breakNum++;
	}

	if (winceValue >= 400 && !isBreak) {
		action->SetActionBreak();
		winceAction = std::make_unique<Boss1Wince>();
		isWince = true;
		winceValue = 0.0f;
	}

	//アクションが終わり次第次に行く
	if (action->GetEnd() && !isWince && !isBreak) {
		SetAction();
	}

	BaseBoss::Update();
}

void Boss1::FrameReset()
{
	action->FrameReset();
	BaseBoss::FrameReset();
}

void Boss1::SetAction()
{
	//初期化
	action.reset();

	Vector3 pos = bossModel->GetPosition();
	Vector3 dist = targetPos - pos;

	//action = std::make_unique<Boss1Bullet1>();
	//return;

	//プレイヤーが遠距離にいる
	if (dist.length() > 100.0f) {
		bool actionRand = RandomInt(100) > 80;
		//前回移動なら必ず攻撃を行う
		//攻撃行動
		if (isMove|| actionRand) {
			isMove = false;
			actionNumber = RandomInt(int(LongAction::middle) + 1, int(LongAction::size) - 1);
			if (actionNumber == int(LongAction::bullet1)) {
				action = std::make_unique<Boss1Bullet1>();
			}else if (actionNumber == int(LongAction::bullet2)) {
				action = std::make_unique<Boss1Bullet2>();
			}
		}
		//移動行動
		else {
			isMove = true;
			actionNumber = RandomInt(0, int(LongAction::middle) - 1);
			if (actionNumber == int(LongAction::move3)) {
				action = std::make_unique<Boss1Move3>();
			}
		}
	}
	//プレイヤーが中距離にいる
	else if (dist.length() > 50.0f) {
		bool actionRand = RandomInt(100) > 50;
		//前回移動なら必ず攻撃を行う
		//攻撃行動
		if (isMove || actionRand) {
			isMove = false;
			actionNumber = RandomInt(int(MediumAction::middle) + 1, int(MediumAction::size) - 1);
			if (actionNumber == int(MediumAction::bullet1)) {
				action = std::make_unique<Boss1Bullet1>();
			}else if (actionNumber == int(MediumAction::bullet2)) {
				action = std::make_unique<Boss1Bullet2>();
			} else if (actionNumber == int(MediumAction::bullet4)) {
				action = std::make_unique<Boss1Bullet4>();
			}
		}
		//移動行動
		else {
			isMove = true;
			actionNumber =RandomInt(0, int(MediumAction::middle) - 1);
			if (actionNumber == int(MediumAction::move3)) {
				action = std::make_unique<Boss1Move1>();
			}

		}
	}
	//プレイヤーが近距離にいる
	else {
		bool actionRand = RandomInt(100) > 50;
		//前回移動なら必ず攻撃を行う
		//攻撃行動
		if (isMove || actionRand) {
			isMove = false;
			actionNumber = RandomInt(int(ShortAction::middle) + 1, int(ShortAction::others) - 1);
			if (actionNumber == int(ShortAction::attack1)) {
				action = std::make_unique<Boss1NearAttack1>();
			}else if (actionNumber == int(ShortAction::bullet2)) {
				action = std::make_unique<Boss1Bullet2>();
			} else if (actionNumber == int(ShortAction::bullet3)) {
				action = std::make_unique<Boss1Bullet3>();
			}
		}
		//移動行動
		else {
			SetShortMoveAction();
		}
	}
}

void Boss1::SetShortMoveAction()
{
	isMove = true;

	//壁との距離が近かった場合の動き
	const float l_size = GameHelper::Instance()->GetStageSize();
	const float l_specifiedValue = 60.0f;
	if (targetPos.x >= l_size - l_specifiedValue || targetPos.z >= l_size - l_specifiedValue ||
		targetPos.x <= l_specifiedValue || targetPos.z <= l_specifiedValue) {
		action = std::make_unique<Boss1Move2>();
		return;
	}

	//その他の移動
	actionNumber = RandomInt(0, int(ShortAction::middle) - 1);
	if (actionNumber == int(ShortAction::move1)) {
		action = std::make_unique<Boss1Move1>();
	}

}
