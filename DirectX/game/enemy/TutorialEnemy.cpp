#include "TutorialEnemy.h"

TutorialEnemy::TutorialEnemy(const DirectX::XMFLOAT3& position)
{
	//オブジェクト生成
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
	//ダメージフラグが立っていたら下ろしておく
	if (isDamage) {
		isDamage = false;
	}

	//オブジェクト更新
	object->Update();
}

void TutorialEnemy::Draw()
{
	//オブジェクト描画
	object->Draw();
}

void TutorialEnemy::Damage()
{
	//ダメージフラグをtrueに
	isDamage = true;
}
