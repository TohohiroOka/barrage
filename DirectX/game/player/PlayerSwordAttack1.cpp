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

	//���v�͂̎g�p�����Z�b�g
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

	//�I�u�W�F�N�g�X�V
	func_[int(state)]();
	object->Update();

	//�Փ˔���p�ϐ��̍X�V
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

	//�A���U���񐔂𑝂₷
	attackNum++;

	//���v�͂�����Ȃ������玟�̍U�����ł����ɔ�����
	if (endurance < useEnduranceNum) {
		//�������Ă��Ȃ������i�K�Ȃ�U�����̂��I������
		if (state == NONE) { isAttackActionEnd = true; }

		return false;
	}

	if (attackNum == 1) {
		//�U��1��
		state = State::ATTACK1;

		attackCollisionData.power = 50;
	}
	else if (attackNum == 2) {
		//�U��2��
		state = State::ATTACK2;
	}
	else if (attackNum == 3) {
		//�U��3��
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
	//���t���[���U����������Ƃ��������̂ŏՓ˔���t���O��������
	isCollisionValid = false;

	object->SetColor(nonAttackColor);
}

void PlayerSwordAttack1::AttackAction1()
{
	//�U��1�ɂ����鎞��
	const int attack1Time = 130;
	timer->Update();

	//���̍U������͉\�ɂ���
	const int nextAttackInputTime = attack1Time - 20;
	if (*timer.get() == nextAttackInputTime) {
		isNextAttackInput = true;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (*timer.get() >= attack1Time) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction2()
{
	//�U��2�ɂ����鎞��
	const int attack2Time = 130;
	timer->Update();

	//���̍U������͉\�ɂ���
	const int nextAttackInputTime = attack2Time - 20;
	if (*timer.get() == nextAttackInputTime) {
		isNextAttackInput = true;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (*timer.get() >= attack2Time) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction3()
{
	//�U��3�ɂ����鎞��
	const int attack3Time = 130;
	timer->Update();

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (*timer.get() >= attack3Time) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}
