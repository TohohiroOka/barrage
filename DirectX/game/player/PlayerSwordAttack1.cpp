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

	//���v�͂̎g�p�����Z�b�g
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

	//�I�u�W�F�N�g�X�V
	func_[int(state)]();
	object->Update();

	//�Փ˔���p�ϐ��̍X�V
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

	//�A���U���񐔂𑝂₷
	attackNum++;

	//���v�͂�����Ȃ������玟�̍U�����ł����ɔ�����
	if (endurance < useEnduranceNum) {
		//�������Ă��Ȃ������i�K�Ȃ�U�����̂��I������
		if (state == NONE) { isAttackActionEnd = true; }

		return false;
	}

	if (attackNum == 1) {
		//�U���O��2��
		state = State::NONATTACK1;

		easeAfterPos = DirectX::XMFLOAT3{ 1, 2, 1 } * object->GetScale().x;

		attackCollisionData.power = 50;
	}
	else if (attackNum == 2) {
		//�U���O��2��
		state = State::NONATTACK2;

		easeAfterPos = DirectX::XMFLOAT3{ -1, 2, 1 } * object->GetScale().x;
	}
	else if (attackNum == 3) {
		//�U���O��3��
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
	//�U���O��1�ɂ����鎞��
	const int nonAttack1Time = 15;
	timer++;
	const float easeTime = (float)timer / nonAttack1Time;

	//�C�[�W���O�œ�����
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (timer >= nonAttack1Time) {
		//�U��1��
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
	//�U��1�ɂ����鎞��
	const int attack1Time = 30;
	timer++;
	const float easeTime = (float)timer / attack1Time;

	//�C�[�W���O�œ�����
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//���̍U������͉\�ɂ���
	const int nextAttackInputTime = 20;
	if (timer == nextAttackInputTime) {
		isNextAttackInput = true;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (timer >= attack1Time) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::NonAttackAction2()
{
	//�U���O��2�ɂ����鎞��
	const int nonAttack2Time = 15;
	timer++;
	const float easeTime = (float)timer / nonAttack2Time;

	//�C�[�W���O�œ�����
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (timer >= nonAttack2Time) {
		//�U��2��
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
	//�U��2�ɂ����鎞��
	const int attack2Time = 30;
	timer++;
	const float easeTime = (float)timer / attack2Time;

	//�C�[�W���O�œ�����
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//���̍U������͉\�ɂ���
	const int nextAttackInputTime = 20;
	if (timer == nextAttackInputTime) {
		isNextAttackInput = true;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (timer >= attack2Time) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::NonAttackAction3()
{
	//�U���O��3�ɂ����鎞��
	const int nonAttack3Time = 15;
	timer++;
	const float easeTime = (float)timer / nonAttack3Time;

	//�C�[�W���O�œ�����
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (timer >= nonAttack3Time) {
		//�U��3��
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
	//�U��3�ɂ����鎞��
	const int attack3Time = 30;
	timer++;
	const float easeTime = (float)timer / attack3Time;

	//�C�[�W���O�œ�����
	Vector3 pos = {};
	pos.x = Easing::OutQuart(easeBeforePos.x, easeAfterPos.x, easeTime);
	pos.y = Easing::OutQuart(easeBeforePos.y, easeAfterPos.y, easeTime);
	pos.z = Easing::OutQuart(easeBeforePos.z, easeAfterPos.z, easeTime);
	object->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (timer >= attack3Time) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}
