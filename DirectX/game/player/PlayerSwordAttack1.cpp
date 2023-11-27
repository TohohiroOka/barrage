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
	//�v���C���[���Z�b�g
	this->player = player;

	//�����蔻������p�I�u�W�F�N�g����
	model = Model::CreateFromOBJ("NormalCube");

	object = Object3d::Create(model.get());
	object->SetScale({ 2.5f, 2.5f, 2.5f });
	object->SetColor(nonAttackColor);

	//�^�C�}�[������
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
	//�U���s�����ő吔�܂ōs���Ă����甲����
	if (attackNum >= maxAttackNum) { return false; }

	//�A���U���񐔂𑝂₷
	attackNum++;

	//���v�͂�����Ȃ������玟�̍U�����ł����ɔ�����
	if (player->GetData()->endurance < useEnduranceNum) {
		//�������Ă��Ȃ������i�K�Ȃ�U�����̂��I������
		if (state == NONE) { isAttackActionEnd = true; }

		return false;
	}

	if (attackNum == 1) {
		//�U��1��
		state = State::ATTACK1;

		//�U���͕ύX
		attackCollisionData.power = 50;

		//�U���ňړ�����Ƃ��p�Ɉړ��X�s�[�h���Z�b�g(���ɃX�s�[�h������ꍇ�͕ύX���Ȃ�)
		player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, attackStartMoveSpeedMax);
		player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, attackStartMoveSpeedMin);
		attackStartMoveSpeed = player->GetData()->moveSpeed;

		//�A�j���[�V�������Z�b�g
		player->GetFbxObject()->AnimationReset();
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK_RIGHT_ANIMATION);
	}
	else if (attackNum == 2) {
		//�U��2��
		state = State::ATTACK2;

		//�U���ňړ�����Ƃ��p�Ɉړ��X�s�[�h���Z�b�g(���ɃX�s�[�h������ꍇ�͕ύX���Ȃ�)
		player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, attackStartMoveSpeedMax);
		player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, attackStartMoveSpeedMin);
		attackStartMoveSpeed = player->GetData()->moveSpeed;

		//�A�j���[�V�������Z�b�g
		player->GetFbxObject()->AnimationReset();
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK_RIGHT_ANIMATION);
	}
	else if (attackNum == 3) {
		//�U��3��
		state = State::ATTACK3;

		//�U���͕ύX
		attackCollisionData.power = 100;

		//�U���ňړ�����Ƃ��p�Ɉړ��X�s�[�h���Z�b�g(���ɃX�s�[�h������ꍇ�͕ύX���Ȃ�)
		player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, attackStartMoveSpeedMax);
		player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, attackStartMoveSpeedMin);
		attackStartMoveSpeed = player->GetData()->moveSpeed;

		//�A�j���[�V�������Z�b�g
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
	//���t���[���U����������Ƃ��������̂ŏՓ˔���t���O��������
	isCollisionValid = false;

	//�U������������
	isHitAttack = true;

	object->SetColor(nonAttackColor);
}

void PlayerSwordAttack1::AttackAction1()
{
	//�^�C�}�[�X�V
	timer->Update();

	//�U���ɍ��킹�ăv���C���[�𓮂���
	MovePlayer(actionChangeStartTime);

	//�U�������ON�ɂ���
	if (*timer.get() == collisionValidStartTime && !isHitAttack) {
		isCollisionValid = true;
	}

	//���̍s������͉\�ɂ���
	if (*timer.get() == actionChangeStartTime) {
		isNextActionInput = true;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (*timer.get() >= attackTime) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction2()
{
	//�^�C�}�[�X�V
	timer->Update();

	//�U���ɍ��킹�ăv���C���[�𓮂���
	MovePlayer(actionChangeStartTime);

	//�U�������ON�ɂ���
	if (*timer.get() == collisionValidStartTime && !isHitAttack) {
		isCollisionValid = true;
	}

	//���̍s������͉\�ɂ���
	if (*timer.get() == actionChangeStartTime) {
		isNextActionInput = true;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (*timer.get() >= attackTime) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction3()
{
	//�^�C�}�[�X�V
	timer->Update();

	//�U���ɍ��킹�ăv���C���[�𓮂���
	MovePlayer(actionChangeStartTime);

	//�U�������ON�ɂ���
	if (*timer.get() == collisionValidStartTime && !isHitAttack) {
		isCollisionValid = true;
	}

	//���̍s������͉\�ɂ���
	if (*timer.get() == actionChangeStartTime) {
		isNextActionInput = true;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (*timer.get() >= attackTime) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::MovePlayer(int moveTime)
{
	//�ړ����Ԃ��߂��Ă����甲����
	if (*timer.get() > moveTime) { return; }

	//�X�s�[�h�𗎂Ƃ��Ă���
	const float easeTime = *timer.get() / (float)moveTime;
	player->GetData()->moveSpeed = Easing::OutSine(attackStartMoveSpeed, 0, easeTime);

	//���x���Z�b�g
	player->GetData()->velocity.x = player->GetData()->moveVec.x * player->GetData()->moveSpeed;
	player->GetData()->velocity.z = player->GetData()->moveVec.z * player->GetData()->moveSpeed;
}
