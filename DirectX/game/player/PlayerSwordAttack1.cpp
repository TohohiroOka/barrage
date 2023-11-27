#include "PlayerSwordAttack1.h"
#include "Player.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Math/Easing/Easing.h"


const DirectX::XMFLOAT4 PlayerSwordAttack1::attackColor = { 1, 0, 0, 0.3f };
const DirectX::XMFLOAT4 PlayerSwordAttack1::nonAttackColor = { 0, 0, 1, 0.3f };
const float PlayerSwordAttack1::attackStartMoveSpeedMax = 1.5f;
const float PlayerSwordAttack1::attackStartMoveSpeedMin = 0.5f;

PlayerSwordAttack1::PlayerSwordAttack1(Player* player)
{
	//プレイヤーをセット
	this->player = player;

	//当たり判定可視化用オブジェクト生成
	model = Model::CreateFromOBJ("NormalCube");

	object = Object3d::Create(model.get());
	object->SetScale({ 2.5f, 2.5f, 2.5f });
	object->SetColor(nonAttackColor);

	//タイマー初期化
	timer = std::make_unique<Engine::Timer>();

	//持久力の使用料をセット
	useEnduranceNum = attackUseEnduranceNum;
	func_.emplace_back([this] {return AttackAction1(); });
	func_.emplace_back([this] {return AttackAction2(); });
	func_.emplace_back([this] {return AttackAction3(); });
}

PlayerSwordAttack1::~PlayerSwordAttack1()
{
}

void PlayerSwordAttack1::Update()
{
	if (state == NONE) { return; }

	//オブジェクト更新
	func_[int(state)]();
	object->Update();

	//衝突判定用変数の更新
	DirectX::XMFLOAT3 objectPos = player->GetFbxObject()->GetAttachPos();
	object->SetPosition(objectPos);
	attackCollisionData.center = { objectPos.x, objectPos.y, objectPos.z, 1 };
	attackCollisionData.radius = object->GetScale().x;
}

void PlayerSwordAttack1::Draw()
{
	if (state == NONE) { return; }

	object->Draw();
}

bool PlayerSwordAttack1::NextAttack()
{
	//攻撃行動を最大数まで行っていたら抜ける
	if (attackNum >= maxAttackNum) { return false; }

	//連続攻撃回数を増やす
	attackNum++;

	//持久力が足りなかったら次の攻撃ができずに抜ける
	if (player->GetData()->endurance < useEnduranceNum) {
		//何もしていない初期段階なら攻撃自体を終了する
		if (state == NONE) { isAttackActionEnd = true; }

		return false;
	}

	if (attackNum == 1) {
		//攻撃1へ
		state = State::ATTACK1;

		//攻撃力変更
		attackCollisionData.power = 50;

		//攻撃で移動するとき用に移動スピードをセット(既にスピードがある場合は変更しない)
		player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, attackStartMoveSpeedMax);
		player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, attackStartMoveSpeedMin);
		attackStartMoveSpeed = player->GetData()->moveSpeed;

		//アニメーションリセット
		player->GetFbxObject()->AnimationReset();
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK_RIGHT_ANIMATION);
	}
	else if (attackNum == 2) {
		//攻撃2へ
		state = State::ATTACK2;

		//攻撃で移動するとき用に移動スピードをセット(既にスピードがある場合は変更しない)
		player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, attackStartMoveSpeedMax);
		player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, attackStartMoveSpeedMin);
		attackStartMoveSpeed = player->GetData()->moveSpeed;

		//アニメーションリセット
		player->GetFbxObject()->AnimationReset();
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK_RIGHT_ANIMATION);
	}
	else if (attackNum == 3) {
		//攻撃3へ
		state = State::ATTACK3;

		//攻撃力変更
		attackCollisionData.power = 100;

		//攻撃で移動するとき用に移動スピードをセット(既にスピードがある場合は変更しない)
		player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, attackStartMoveSpeedMax);
		player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, attackStartMoveSpeedMin);
		attackStartMoveSpeed = player->GetData()->moveSpeed;

		//アニメーションリセット
		player->GetFbxObject()->AnimationReset();
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK_RIGHT_ANIMATION);
	}

	isNextActionInput = false;
	timer->Reset();
	object->SetColor(nonAttackColor);
	isCollisionValid = false;
	isHitAttack = false;
	object->SetColor(attackColor);

	return true;
}

void PlayerSwordAttack1::AttackCollision()
{
	//毎フレーム攻撃が当たるとおかしいので衝突判定フラグを下げる
	isCollisionValid = false;

	//攻撃が当たった
	isHitAttack = true;

	object->SetColor(nonAttackColor);
}

void PlayerSwordAttack1::AttackAction1()
{
	//タイマー更新
	timer->Update();

	//攻撃に合わせてプレイヤーを動かす
	MovePlayer(actionChangeStartTime);

	//攻撃判定をONにする
	if (*timer.get() == collisionValidStartTime && !isHitAttack) {
		isCollisionValid = true;
	}

	//次の行動を入力可能にする
	if (*timer.get() == actionChangeStartTime) {
		isNextActionInput = true;
	}

	//タイマーが指定した時間になったら
	if (*timer.get() >= attackTime) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction2()
{
	//タイマー更新
	timer->Update();

	//攻撃に合わせてプレイヤーを動かす
	MovePlayer(actionChangeStartTime);

	//攻撃判定をONにする
	if (*timer.get() == collisionValidStartTime && !isHitAttack) {
		isCollisionValid = true;
	}

	//次の行動を入力可能にする
	if (*timer.get() == actionChangeStartTime) {
		isNextActionInput = true;
	}

	//タイマーが指定した時間になったら
	if (*timer.get() >= attackTime) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction3()
{
	//タイマー更新
	timer->Update();

	//攻撃に合わせてプレイヤーを動かす
	MovePlayer(actionChangeStartTime);

	//攻撃判定をONにする
	if (*timer.get() == collisionValidStartTime && !isHitAttack) {
		isCollisionValid = true;
	}

	//次の行動を入力可能にする
	if (*timer.get() == actionChangeStartTime) {
		isNextActionInput = true;
	}

	//タイマーが指定した時間になったら
	if (*timer.get() >= attackTime) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::MovePlayer(int moveTime)
{
	//移動時間を過ぎていたら抜ける
	if (*timer.get() > moveTime) { return; }

	//スピードを落としていく
	const float easeTime = *timer.get() / (float)moveTime;
	player->GetData()->moveSpeed = Easing::OutSine(attackStartMoveSpeed, 0, easeTime);

	//速度をセット
	player->GetData()->velocity.x = player->GetData()->moveVec.x * player->GetData()->moveSpeed;
	player->GetData()->velocity.z = player->GetData()->moveVec.z * player->GetData()->moveSpeed;
}
