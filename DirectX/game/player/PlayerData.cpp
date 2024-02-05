#include "PlayerData.h"
#include "GameHelper.h"

int PlayerData::dashUseEndurance = 1;
int PlayerData::avoidUseEndurance = 10;
int PlayerData::jumpUseEndurance = 10;
int PlayerData::blinkUseEndurance = 10;

PlayerData::PlayerData()
{
	action = PlayerActionName::MOVE_NORMAL;

	pos = { GameHelper::Instance()->GetStageSize() / 2.0f, 5.0f,GameHelper::Instance()->GetStageSize() / 5.0f };
	moveVec = { 0, 0, 1 };

	onGround = false;
	fallSpeed = 0;

	isMoveKey = false;
	isMovePad = false;

	actionInput.isMove = true;
	actionInput.isJump = true;
	actionInput.isLightAttack = true;
	actionInput.isStrongAttack = true;
	actionInput.isAvoid = true;
	actionInput.isBlink = true;

	isDash = false;
	moveSpeed = 0.0f;
	
	jumpMaxNum = 2;
	jumpCount = 0;
	isInputJump = false;

	isEnemyAttackAvoid = false;
	isEnemyAttackAvoidJudge = false;
	
	maxHP = 1;
	HP = maxHP;
	
	maxEndurance = 400;
	endurance = maxEndurance;

	isNoGravity = false;
	isNoDamage = false;
	isNoDead = false;
	isUseEndurance = true;
	isDead = false;
}

PlayerData::~PlayerData()
{
}
