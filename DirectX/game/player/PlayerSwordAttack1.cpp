#include "PlayerSwordAttack1.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Math/Easing/Easing.h"

const DirectX::XMFLOAT4 PlayerSwordAttack1::attackColor = { 1, 0, 0, 0.3f };
const DirectX::XMFLOAT4 PlayerSwordAttack1::nonAttackColor = { 0, 0, 1, 0.3f };

PlayerSwordAttack1::PlayerSwordAttack1(Base3D* attacker)
{
	model = Model::CreateFromOBJ("NormalCube");

	object = Object3d::Create(model.get());
	object->SetParent(attacker);
	object->SetScale({ 5, 5, 5 });
	object->SetPosition(DirectX::XMFLOAT3{ 0, 1, 1 } * object->GetScale().x);
	object->SetColor(nonAttackColor);

	attackCollisionData.power = 50;

	//持久力の使用料をセット
	useEnduranceNum = attackUseEnduranceNum;

	func_.emplace_back([this] {return NonAttackAction1(); });
	func_.emplace_back([this] {return AttackAction1(); });
	func_.emplace_back([this] {return NonAttackAction2(); });
	func_.emplace_back([this] {return AttackAction2(); });
	func_.emplace_back([this] {return NonAttackAction3(); });
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
	Vector3 objectPos = object->GetWorldPosition();
	attackCollisionData.center = { objectPos.x, objectPos.y, objectPos.z, 1 };
	attackCollisionData.radius = object->GetScale().x;
}

void PlayerSwordAttack1::Draw()
{
	if (state == NONE) { return; }

	object->Draw();
}

void PlayerSwordAttack1::DrawLightView()
{
	if (state == NONE) { return; }

	object->DrawLightView();
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
		//攻撃前兆2へ
		state = State::NONATTACK1;

		easeAfterPos = DirectX::XMFLOAT3{ 1, 2, 1 } * object->GetScale().x;

		attackCollisionData.power = 50;
	}
	else if (attackNum == 2) {
		//攻撃前兆2へ
		state = State::NONATTACK2;

		easeAfterPos = DirectX::XMFLOAT3{ -1, 2, 1 } * object->GetScale().x;
	}
	else if (attackNum == 3) {
		//攻撃前兆3へ
		state = State::NONATTACK3;

		easeAfterPos = DirectX::XMFLOAT3{ 0, 2, 1 } * object->GetScale().x;

		attackCollisionData.power = 100;
	}

	isNextAttackInput = false;
	timer = 0;
	object->SetColor(nonAttackColor);
	isCollisionValid = false;

	easeBeforePos = object->GetPosition();

	return true;
}

void PlayerSwordAttack1::NonAttackAction1()
{
	//攻撃前兆1にかかる時間
	const int nonAttack1Time = 15;
	timer++;
	const float easeTime = (float)timer / nonAttack1Time;

	//イージングで動かす
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (timer >= nonAttack1Time) {
		//攻撃1へ
		state = State::ATTACK1;
		timer = 0;
		object->SetColor(attackColor);
		isCollisionValid = true;

		easeBeforePos = object->GetPosition();
		easeAfterPos = DirectX::XMFLOAT3{ 0, -0.5f, 1 } * object->GetScale().x;
	}
}

void PlayerSwordAttack1::AttackAction1()
{
	//攻撃1にかかる時間
	const int attack1Time = 30;
	timer++;
	const float easeTime = (float)timer / attack1Time;

	//イージングで動かす
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//次の攻撃を入力可能にする
	const int nextAttackInputTime = 20;
	if (timer == nextAttackInputTime) {
		isNextAttackInput = true;
	}

	//タイマーが指定した時間になったら
	if (timer >= attack1Time) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::NonAttackAction2()
{
	//攻撃前兆2にかかる時間
	const int nonAttack2Time = 15;
	timer++;
	const float easeTime = (float)timer / nonAttack2Time;

	//イージングで動かす
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (timer >= nonAttack2Time) {
		//攻撃2へ
		state = State::ATTACK2;
		timer = 0;
		object->SetColor(attackColor);
		isCollisionValid = true;

		easeBeforePos = object->GetPosition();
		easeAfterPos = DirectX::XMFLOAT3{ 0, -0.5f, 1 } *object->GetScale().x;
	}
}

void PlayerSwordAttack1::AttackAction2()
{
	//攻撃2にかかる時間
	const int attack2Time = 30;
	timer++;
	const float easeTime = (float)timer / attack2Time;

	//イージングで動かす
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//次の攻撃を入力可能にする
	const int nextAttackInputTime = 20;
	if (timer == nextAttackInputTime) {
		isNextAttackInput = true;
	}

	//タイマーが指定した時間になったら
	if (timer >= attack2Time) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::NonAttackAction3()
{
	//攻撃前兆3にかかる時間
	const int nonAttack3Time = 15;
	timer++;
	const float easeTime = (float)timer / nonAttack3Time;

	//イージングで動かす
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (timer >= nonAttack3Time) {
		//攻撃3へ
		state = State::ATTACK3;
		timer = 0;
		object->SetColor(attackColor);
		isCollisionValid = true;

		easeBeforePos = object->GetPosition();
		easeAfterPos = DirectX::XMFLOAT3{ 0, -0.5f, 1 } *object->GetScale().x;
	}
}

void PlayerSwordAttack1::AttackAction3()
{
	//攻撃3にかかる時間
	const int attack3Time = 30;
	timer++;
	const float easeTime = (float)timer / attack3Time;

	//イージングで動かす
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (timer >= attack3Time) {
		//攻撃行動終了
		isAttackActionEnd = true;
	}
}
