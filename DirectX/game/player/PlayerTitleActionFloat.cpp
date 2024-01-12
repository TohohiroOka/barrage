#include "PlayerTitleActionFloat.h"
#include "Player.h"
#include "Math/Easing/Easing.h"

PlayerTitleActionFloat::PlayerTitleActionFloat(Player* player, const Vector3& portalPos)
	:PlayerActionBase(player)
{
	//イージング用情報をセット
	easeBeforePos = player->GetData()->pos;
	Vector3 portalDistance = { 0, 0, -15 };
	easeAfterPos = portalPos + portalDistance;

	easeBeforeRota = player->GetData()->rota;
	easeAfterRota = { 15, 0, 0 };
	Rotate360(easeAfterRota.x);
	Rotate360(easeAfterRota.y);
	Rotate360(easeAfterRota.z);
	//y軸回転角が180以上の場合は逆回転になるように調整
	if (easeBeforeRota.y >= 180) {
		easeBeforeRota.y -= 360;
	}

	//待機アニメーションに変更
	if (!(player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::STAY_ANIMATION)) {
		player->GetFbxObject()->AnimationReset();
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
	}

	//タイマー生成
	floatTimer = std::make_unique<Engine::Timer>();

	//重力オフ
	player->GetData()->isNoGravity = true;
	//速度を0にする
	player->GetData()->velocity = {0,0,0};

	//予め次の行動を設定しておく(終了後はポータルに入る)
	nextAction = PlayerActionName::TITLE_INTO_PORTAL;
}

PlayerTitleActionFloat::~PlayerTitleActionFloat()
{
}

void PlayerTitleActionFloat::Update()
{
	//浮遊
	Float();
}

void PlayerTitleActionFloat::Float()
{
	const float floatTime = 180;
	floatTimer->Update();
	const float time = *floatTimer.get() / floatTime;
	player->GetData()->pos.x = Easing::InOutQuart(easeBeforePos.x, easeAfterPos.x, time);
	player->GetData()->pos.y = Easing::InOutQuart(easeBeforePos.y, easeAfterPos.y, time);
	player->GetData()->pos.z = Easing::InOutQuart(easeBeforePos.z, easeAfterPos.z, time);

	player->GetData()->rota.x = Easing::InOutQuart(easeBeforeRota.x, easeAfterRota.x, time);
	player->GetData()->rota.y = Easing::InOutQuart(easeBeforeRota.y, easeAfterRota.y, time);
	player->GetData()->rota.z = Easing::InOutQuart(easeBeforeRota.z, easeAfterRota.z, time);
	player->GetFbxObject()->SetRotation(player->GetData()->rota);

	if (*floatTimer.get() >= floatTime) {
		isActionEnd = true;

		//予約していた次の行動をセット
		player->GetData()->action = nextAction;
	}
}
