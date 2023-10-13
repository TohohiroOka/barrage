#include "PlayerSwordAttack1.h"

PlayerSwordAttack1::PlayerSwordAttack1(Base3D* attacker)
{
	model = Model::CreateFromOBJ("NormalCube");

	object = Object3d::Create(model.get());
	object->SetParent(attacker);
	object->SetScale({ 5, 5, 5 });
	object->SetPosition({ 0, object->GetScale().y, object->GetScale().z});
	object->SetColor({ 0, 0, 1, 0.3f });

	attackCollisionData.power = 10;

	func_.emplace_back([this] {return NonAttackAction(); });
	func_.emplace_back([this] {return AttackAction(); });
}

PlayerSwordAttack1::~PlayerSwordAttack1()
{
}

void PlayerSwordAttack1::Update()
{
	//オブジェクト更新
	func_[int(state)]();
	object->Update();

	//衝突判定用変数の更新
	Vector3 objectPos = object->GetWorldPosition();
	attackCollisionData.center = { objectPos.x, objectPos.y, objectPos.z, 1 };
	attackCollisionData.radius = object->GetScale().x;
}

void PlayerSwordAttack1::Draw()
{
	object->Draw();
}

void PlayerSwordAttack1::NonAttackAction()
{
	//攻撃をしない時間
	const int nonAttackTime = 60;
	timer++;

	//タイマーが指定した時間になったら
	if (timer >= nonAttackTime) {
		//攻撃
		state = State::ATTACK;
		timer = 0;
		object->SetColor({ 1, 0, 0, 0.3f });
		isCollisionValid = true;
	}
}

void PlayerSwordAttack1::AttackAction()
{
	//攻撃をする時間
	const int attackTime = 60;
	timer++;

	//タイマーが指定した時間になったら
	if (timer >= attackTime) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}
