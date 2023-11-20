#include "PlayerSwordAttack1.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Math/Easing/Easing.h"

const DirectX::XMFLOAT4 PlayerSwordAttack1::attackColor = { 1, 0, 0, 0.3f };
const DirectX::XMFLOAT4 PlayerSwordAttack1::nonAttackColor = { 0, 0, 1, 0.3f };

PlayerSwordAttack1::PlayerSwordAttack1(std::function<DirectX::XMFLOAT3()> getSwordPos)
{
	getSwordPos_ = getSwordPos;

	model = Model::CreateFromOBJ("NormalCube");

	object = Object3d::Create(model.get());
	object->SetScale({ 2.5f, 2.5f, 2.5f });
	object->SetColor(nonAttackColor);

	attackCollisionData.power = 50;

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
	DirectX::XMFLOAT3 objectPos = getSwordPos_();
	object->SetPosition(objectPos);
	attackCollisionData.center = { objectPos.x, objectPos.y, objectPos.z, 1 };
	attackCollisionData.radius = object->GetScale().x;
}

void PlayerSwordAttack1::Draw()
{
	if (state == NONE) { return; }

	object->Draw();
}

bool PlayerSwordAttack1::NextAttack(int endurance)
{
	if (attackNum >= maxAttackNum) { return false; }

	//連続攻撃回数を増やす
	attackNum++;

	//持久力が足りなかったら次の攻撃ができずに抜ける
	if (endurance < useEnduranceNum) {
		//何もしていない初期段階なら攻撃自体を終了する
		if (state == NONE) { isAttackActionEnd = true; }

		return false;
	}

	if (attackNum == 1) {
		//攻撃1へ
		state = State::ATTACK1;

		attackCollisionData.power = 50;
	}
	else if (attackNum == 2) {
		//攻撃2へ
		state = State::ATTACK2;
	}
	else if (attackNum == 3) {
		//攻撃3へ
		state = State::ATTACK3;

		attackCollisionData.power = 100;
	}

	isNextAttackInput = false;
	timer->Reset();
	object->SetColor(nonAttackColor);
	isCollisionValid = true;
	object->SetColor(attackColor);

	return true;
}

void PlayerSwordAttack1::AttackCollision()
{
	//毎フレーム攻撃が当たるとおかしいので衝突判定フラグを下げる
	isCollisionValid = false;

	object->SetColor(nonAttackColor);
}

void PlayerSwordAttack1::AttackAction1()
{
	//攻撃1にかかる時間
	const int attack1Time = 130;
	timer->Update();

	//次の攻撃を入力可能にする
	const int nextAttackInputTime = attack1Time - 20;
	if (*timer.get() == nextAttackInputTime) {
		isNextAttackInput = true;
	}

	//タイマーが指定した時間になったら
	if (*timer.get() >= attack1Time) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction2()
{
	//攻撃2にかかる時間
	const int attack2Time = 130;
	timer->Update();

	//次の攻撃を入力可能にする
	const int nextAttackInputTime = attack2Time - 20;
	if (*timer.get() == nextAttackInputTime) {
		isNextAttackInput = true;
	}

	//タイマーが指定した時間になったら
	if (*timer.get() >= attack2Time) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction3()
{
	//攻撃3にかかる時間
	const int attack3Time = 130;
	timer->Update();

	//タイマーが指定した時間になったら
	if (*timer.get() >= attack3Time) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}
