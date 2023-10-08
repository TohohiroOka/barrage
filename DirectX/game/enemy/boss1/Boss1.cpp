#include "Boss1.h"
#include "Boss1NearAttack1.h"

Boss1::Boss1()
{
	BaseBoss::Initialize();

	model = Model::CreateFromOBJ("NormalCube");
	boss = Object3d::Create(model.get());
	boss->SetParent(center.get());

	//boss->SetPosition({150.0f,120.0f,150.0f});
	boss->SetScale({ 5.0f,5.0f ,5.0f });

	BaseAction::SetBossPtr(this);

	action = std::make_unique<Boss1NearAttack1>();
}

void Boss1::Update()
{
	//�A�N�V�������I��莟�掟�ɍs��
	if (action->End()) {
		SetAction();
	}

	action->Update();

	BaseBoss::Update();
	boss->Update();
}

void Boss1::Draw()
{
	action->Draw();

	boss->Draw();
}

void Boss1::SetAction()
{
	//������
	action->~BaseAction();
	action = nullptr;

	//�s���I���̏ꍇ����������
	//���݂̓e�X�g�̂��߈�����[�v����
	actionNumber = 0;
	isAction = true;

	action = std::make_unique<Boss1NearAttack1>();
}
