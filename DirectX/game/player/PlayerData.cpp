#include "PlayerData.h"

PlayerData::PlayerData()
{
	action = PlayerActionName::MOVENORMAL;

	pos = { 100.0f,50.0f,100.0f };
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
