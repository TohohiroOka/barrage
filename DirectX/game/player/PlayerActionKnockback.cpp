#include "PlayerActionKnockback.h"
#include "Player.h"

PlayerActionKnockback::PlayerActionKnockback(Player* player, const Vector3& subjectPos, int power)
	: PlayerActionBase(player)
{
	//攻撃対象と自分のベクトルを算出
	knockbackVec = player->GetData()->pos - subjectPos;
	//ノックバックの強さをセット(仮で食らったダメージ量 / 10)
	knockbackPower = (float)power / 10;

	//予め次の行動を設定しておく(終了後は通常移動)
	nextAction = PlayerActionName::MOVENORMAL;
}

PlayerActionKnockback::~PlayerActionKnockback()
{
}

void PlayerActionKnockback::Update()
{
	//ノックバック
	Knockback();
}

void PlayerActionKnockback::Knockback()
{
	//ノックバック
	player->GetData()->velocity = knockbackVec.normalize() * knockbackPower;

	//ノックバックを弱くしていく
	knockbackPower -= 0.05f * GameHelper::Instance()->GetGameSpeed();

	//ノックバックによる移動がなくなったらノックバック行動終了
	if (knockbackPower < 0) {
		isActionEnd = true;

		//予約していた次の行動をセット
		player->GetData()->action = nextAction;
	}
}
