#include "Boss1Bullet1.h"
#include "GameHelper.h"
#include "../BaseBoss.h"

Boss1Bullet1::Boss1Bullet1()
{
	model = Model::CreateFromOBJ("bullet");
	BaseBullet::CreateInstance(model.get());
}

Boss1Bullet1::~Boss1Bullet1()
{
	BaseBullet::Reset();
}

void Boss1Bullet1::Update()
{
	const float maxTimer = 100.0f;

	if (int(timer) % 5 == 0 && timer < maxTimer) {
		for (int i = 0; i < 3; i++) {
			AddBullet();
		}
	}

	//falseÇ»ÇÁè¡Ç∑
	bullet.remove_if([](BaseBullet& x) {
		return !x.GetIsAlive();
		}
	);

	//çXêVèàóù
	for (std::forward_list<BaseBullet>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		it->Update();
	}

	timer++;

	//èIóπ
	if (std::distance(bullet.begin(), bullet.end()) <= 0) {
		isEnd=true;
	}
}

void Boss1Bullet1::Draw()
{
	for (std::forward_list<BaseBullet>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		it->Draw();
	}

	BaseBullet::StaticDraw();
}

void Boss1Bullet1::FrameReset()
{
	BaseBullet::FrameReset();
}

void Boss1Bullet1::GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info)
{
}

void Boss1Bullet1::AddBullet()
{
	Vector3 bossPos = boss->GetCenter()->GetPosition();
	Vector3 targetPos = boss->GetTargetPos();

	float randomX = float(GameHelper::Instance()->RandomInt(300) - 150) / 10.0f;
	float randomZ = float(GameHelper::Instance()->RandomInt(300) - 150) / 10.0f;

	bossPos += {0.0f, 30.0f, 0.0f};
	targetPos += {randomX, 0.0f, randomZ};
	Vector3 vec = targetPos - bossPos;
	Vector3 normalVec = vec.normalize();

	bullet.emplace_front(BaseBullet(bossPos, { 1.0f,1.0f ,1.0f }, { 0.0f,0.0f ,0.0f }, normalVec * 20.0f));
}
