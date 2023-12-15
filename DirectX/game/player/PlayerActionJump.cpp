#include "PlayerActionJump.h"
#include "Player.h"
#include "engine/Audio/Audio.h"

float PlayerActionJump::jumpPower = 1.0f;

PlayerActionJump::PlayerActionJump(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(PlayerData::jumpUseEndurance, 45, true); //持久力を使用

	player->GetData()->onGround = false; //地面との接地を解除
	player->GetData()->isInputJump = true; //ジャンプ入力状態にする
	player->GetData()->velocity.y = jumpPower; //上向きに跳ぶ
	player->GetData()->fallSpeed = 0; //跳ぶので落下速度を0に
	player->GetData()->jumpCount++; //ジャンプ回数を増やす

	//ジャンプアニメーションに変更
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::JUMP_ANIMATION);

	//予め次の行動を設定しておく(終了後は通常移動)
	nextAction = PlayerActionName::MOVENORMAL;

	//ジャンプ音再生
	Audio::Instance()->SoundPlayWava(Sound::SoundName::jump, false, 0.1f);
}

PlayerActionJump::~PlayerActionJump()
{
}

void PlayerActionJump::Update()
{
	//ジャンプをするだけなので一瞬で終了
	isActionEnd = true;
	//予約していた次の行動をセット
	player->GetData()->action = nextAction;
}
