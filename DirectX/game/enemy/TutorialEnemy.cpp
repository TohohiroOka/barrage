#include "TutorialEnemy.h"

TutorialEnemy::TutorialEnemy(const DirectX::XMFLOAT3& position)
{
	//�I�u�W�F�N�g����
	model = Model::CreateFromOBJ("warp");
	object = Object3d::Create(model.get());
	object->SetPosition(position);
	const float size = 10.0f;
	object->SetScale({ size, size, size });
	object->SetShadowMap(true);
}

TutorialEnemy::~TutorialEnemy()
{
}

void TutorialEnemy::Update()
{
	//�_���[�W�t���O�������Ă����牺�낵�Ă���
	if (isDamage) {
		isDamage = false;
	}

	//�I�u�W�F�N�g�X�V
	object->Update();
}

void TutorialEnemy::Draw()
{
	//�I�u�W�F�N�g�`��
	object->Draw();
}

void TutorialEnemy::Damage()
{
	//�_���[�W�t���O��true��
	isDamage = true;
}
