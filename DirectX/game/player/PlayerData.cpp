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

	moveSpeed = 0.0f;
	
	jumpMaxNum = 2;
	jumpCount = 0;
	isInputJump = false;
	
	maxHP = 100;
	HP = maxHP;
	
	maxEndurance = 300;
	endurance = maxEndurance;

	isNoGravity = false;
	isNoDamage = false;
	isDead = false;
}

PlayerData::~PlayerData()
{
}
