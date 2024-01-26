#include "PlayerSwordAttack1.h"
#include "Player.h"
#include "Camera/GameCamera.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Math/Easing/Easing.h"
#include "engine/Audio/Audio.h"
#include "system/GameInputManager.h"

using namespace DirectX;

int PlayerSwordAttack1::attackUseEnduranceNum = 20;
int PlayerSwordAttack1::attackPower = 50;
const DirectX::XMFLOAT4 PlayerSwordAttack1::attackColor = { 1, 0, 0, 0.3f };
const DirectX::XMFLOAT4 PlayerSwordAttack1::nonAttackColor = { 0, 0, 1, 0.3f };
const float PlayerSwordAttack1::attackStartMoveSpeedMax = 1.5f;
const float PlayerSwordAttack1::attackStartMoveSpeedMin = 0.5f;

PlayerSwordAttack1::PlayerSwordAttack1(Player* player)
{
	//�v���C���[���Z�b�g
	this->player = player;
	player->GetSword()->GetSlashEffect()->SetIsParticleEmit(true); //���Ŏa�����o�������J�n���Ă����B
	player->GetSword()->GetSlashEffect()->SetStartColor(DirectX::XMFLOAT4{ 0.02f, 0.05f, 0.2f, 1.0f });
	//�����蔻������p�I�u�W�F�N�g����
	model = Model::CreateFromOBJ("bullet");

	object = Object3d::Create(model.get());
	object->SetScale({ 1.2f, 1.2f, 1.2f });
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

	//���t���[��������
	player->GetData()->isMoveKey = false;
	player->GetData()->isMovePad = false;

	//�v���C���[��]
	PlayerChangeRotate();

	//�I�u�W�F�N�g�X�V
	func_[int(state)]();
	object->Update();

	//�Փ˔���p�ϐ��̍X�V
	DirectX::XMFLOAT3 swordTopButtonVec = player->GetFbxObject()->GetAttachPos("sword2") - player->GetFbxObject()->GetAttachPos("sword1"); //���悩�玝�����܂ł̃x�N�g��
	DirectX::XMFLOAT3 objectPos = player->GetFbxObject()->GetAttachPos("sword1") + (swordTopButtonVec * 0.75f);
	object->SetPosition(objectPos);
	attackCollisionData.startPosition = player->GetFbxObject()->GetAttachPos("sword1");
	attackCollisionData.endPosition = player->GetFbxObject()->GetAttachPos("sword2");
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

	//�A�j���[�V�������Z�b�g
	player->GetFbxObject()->AnimationReset();

	if (attackNum == 1) {
		//�U��1��
		state = State::ATTACK1;

		//�U���͕ύX
		attackCollisionData.power = attackPower;

		//�U��1�A�j���[�V����
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK1_ANIMATION);
	}
	else if (attackNum == 2) {
		//�U��2��
		state = State::ATTACK2;

		//�U��2�A�j���[�V����
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK2_ANIMATION);
	}

	else if (attackNum == 3) {
		//�U��3��
		state = State::ATTACK3;

		//�U��3�A�j���[�V����
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ATTACK3_ANIMATION);
	}

	//�U���ňړ�����Ƃ��p�Ɉړ��X�s�[�h���Z�b�g(���ɃX�s�[�h������ꍇ�͕ύX���Ȃ�)
	player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, attackStartMoveSpeedMax);
	player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, attackStartMoveSpeedMin);
	attackStartMoveSpeed = player->GetData()->moveSpeed;


	//�U�����Đ�
	Audio::Instance()->SoundPlayWava(Sound::SoundName::attack, false, 0.1f);

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
	MovePlayer(actionChangeStartTime1);

	//�U�������ON�ɂ���
	if (*timer.get() == collisionValidStartTime1 && !isHitAttack) {
		isCollisionValid = true;
	}

	//���̍s������͉\�ɂ���
	if (*timer.get() == actionChangeStartTime1) {
		isNextActionInput = true;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (*timer.get() >= attackTime1) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction2()
{
	//�^�C�}�[�X�V
	timer->Update();

	//�U���ɍ��킹�ăv���C���[�𓮂���
	MovePlayer(actionChangeStartTime2);

	//�U�������ON�ɂ���
	if (*timer.get() == collisionValidStartTime2 && !isHitAttack) {
		isCollisionValid = true;
	}

	//���̍s������͉\�ɂ���
	if (*timer.get() == actionChangeStartTime2) {
		isNextActionInput = true;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (*timer.get() >= attackTime2) {
		//�U���s���I��
		isAttackActionEnd = true;
	}
}

void PlayerSwordAttack1::AttackAction3()
{
	//�^�C�}�[�X�V
	timer->Update();

	//�U���ɍ��킹�ăv���C���[�𓮂���
	MovePlayer(actionChangeStartTime3);

	//�U�������ON�ɂ���
	if (*timer.get() == collisionValidStartTime3 && !isHitAttack) {
		isCollisionValid = true;
	}

	//���̍s������͉\�ɂ���
	if (*timer.get() == actionChangeStartTime3) {
		isNextActionInput = true;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (*timer.get() >= attackTime3) {
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

void PlayerSwordAttack1::PlayerChangeRotate()
{
	DirectInput* input = DirectInput::GetInstance();

	//�L�[���͂𔻒�
	player->GetData()->isMoveKey = (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveRight).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveLeft).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveForward).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveBack).key));

	//������x�X�e�B�b�N���X���Ȃ��ƃp�b�h���͔��肵�Ȃ�
	const XMFLOAT2 padIncline = GameInputManager::GetPadLStickIncline();
	player->GetData()->isMovePad = (fabsf(padIncline.x) >= GameInputManager::GetPadStickInputIncline() || fabsf(padIncline.y) >= GameInputManager::GetPadStickInputIncline());


	//���͔��肪�Ȃ���Δ�����
	if (!(player->GetData()->isMoveKey || player->GetData()->isMovePad)) { return; }

	Vector3 inputMoveVec{};

	if (player->GetData()->isMoveKey) {
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveRight).key)) {
			inputMoveVec.x = 1;
		}
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveLeft).key)) {
			inputMoveVec.x = -1;
		}
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveForward).key)) {
			inputMoveVec.z = 1;
		}
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveBack).key)) {
			inputMoveVec.z = -1;
		}
	}
	if (player->GetData()->isMovePad) {
		//�p�b�h�X�e�B�b�N�̕������x�N�g����
		inputMoveVec.x = GameInputManager::GetPadLStickIncline().x;
		inputMoveVec.z = GameInputManager::GetPadLStickIncline().y;
	}

	//�x�N�g�����J�����̌X���ŉ�]������
	inputMoveVec.normalize();
	const float cameraRotaRadian = XMConvertToRadians(-player->GetGameCamera()->GetCameraRota().y);
	player->GetData()->moveVec.x = inputMoveVec.x * cosf(cameraRotaRadian) - inputMoveVec.z * sinf(cameraRotaRadian);
	player->GetData()->moveVec.z = inputMoveVec.x * sinf(cameraRotaRadian) + inputMoveVec.z * cosf(cameraRotaRadian);

	//�i�s�����������悤�ɂ���
	const Vector3 moveRotaVelocity = { player->GetData()->moveVec.x, 0, player->GetData()->moveVec.z }; //�v���C���[��]�ɃW�����v�͊֌W�Ȃ��̂ŁA���xY��0�ɂ��Ă���
	player->SetMoveRotate(moveRotaVelocity, 5.0f);
}
