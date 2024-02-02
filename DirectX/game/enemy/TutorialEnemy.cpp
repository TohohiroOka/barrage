#include "TutorialEnemy.h"
#include "game/player/PlayerData.h"

TutorialEnemy::TutorialEnemy(const DirectX::XMFLOAT3& position, PlayerData* _playerData)
{
	//オブジェクト生成
	model = Model::CreateFromOBJ("tutorialEnemy");
	object = Object3d::Create(model.get());
	object->SetPosition(position);
	const float size = 10.0f;
	object->SetScale({ size, size, size });
	object->SetShadowMap(true);

	bulletShotTimer = std::make_unique<Engine::Timer>();
	TutorialEnemyBullet::StaticInitialize();

	playerData = _playerData;

	damageTimer = std::make_unique<Engine::Timer>();
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

	DamageEffect();

	if (isBulletShot) {
		bulletShotTimer->Update();
		const int bulletShotInterval = 120;
		if (*bulletShotTimer.get() >= bulletShotInterval) {
			AddBullet();
			bulletShotTimer->Reset();
		}
	}

	for (std::forward_list<TutorialEnemyBullet>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		it->Update();
	}

	//削除
	bullet.remove_if([](TutorialEnemyBullet& x) {
		return !x.GetIsAlive();
		}
	);

	SetPlayerDirection();

	//オブジェクト更新
	object->Update();
}

void TutorialEnemy::Draw()
{
	//オブジェクト描画
	object->Draw();

	TutorialEnemyBullet::Draw();
}

void TutorialEnemy::Damage()
{
	//ダメージフラグをtrueに
	isDamage = true;

	isDamegeColor = true;

	damageTimer->Reset();
}

void TutorialEnemy::DamageEffect()
{
	if (isDamegeColor) {
		if (*damageTimer.get() % 5 == 0) {
			isDamageRed = !isDamageRed;
		}

		if (isDamageRed) {
			object->SetColor({ 0.9f,0.1f,0.1f,1.0f });
		} else {
			object->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}
		damageTimer->Update();

		if (*damageTimer.get() < 200.0f) { return; }
		object->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		isDamegeColor = false;
		isDamageRed = false;
		damageTimer->Reset();
	}
}

void TutorialEnemy::AddBullet()
{
	Vector3 playerPos = playerData->pos;
	Vector3 moveVec = playerPos - Vector3(object->GetPosition());
	moveVec = moveVec.normalize();

	bullet.emplace_front(TutorialEnemyBullet(object->GetPosition(), moveVec * 3.0f));
}

void TutorialEnemy::FrameReset()
{
	TutorialEnemyBullet::FrameReset();
}

void TutorialEnemy::GetAttackCollision(std::vector<Sphere>& _info)
{
	for (auto& i : bullet) {
		Sphere add;
		add.center = { i.GetPos().x,i.GetPos().y,i.GetPos().z };
		add.radius = 0.5f;
		_info.emplace_back(add);
	}
}

void TutorialEnemy::SetPlayerDirection()
{
	Vector3 rota = VelocityRotate(Vector3(object->GetPosition()) - playerData->pos);
	//rota.x += 90.0f;
	object->SetRotation(rota);
}

void TutorialEnemy::DeleteBullet(std::vector<int> _deleteNum)
{
	int num = -1;
	int vecNum = 0;
	for (std::forward_list<TutorialEnemyBullet>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		num++;
		if (num != _deleteNum[vecNum]) { continue; }
		it->SetIsAlive(false);
		vecNum++;
		if (_deleteNum.size() == vecNum) { break; }
	}
}