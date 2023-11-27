#include "Boss1NearAttack1.h"
#include "../boss1/Boss1Model.h"
#include "../BaseBoss.h"
#include "GameHelper.h"
#include "../Math/Easing/Easing.h"

using namespace DirectX;

Boss1NearAttack1::Boss1NearAttack1()
{
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::nearAttak_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::nearAttak_start));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetIsRoop(false);

	useCollision = UseCollision::box;
	model = Model::CreateFromOBJ("cone");
	for (auto& i : instanceObject) {
		i = InstanceObject::Create(model.get());
	}
	state = State::start;

	timer = std::make_unique<Engine::Timer>();
	oldtime = timer->GetTime() - 1.0f;
	isEnd = false;

	hitTimer = std::make_unique<Engine::Timer>();

	func_.emplace_back([this] {return StartMove(); });
	func_.emplace_back([this] {return Attack(); });
	func_.emplace_back([this] {return Finish(); });
}

void Boss1NearAttack1::Update()
{
	//動き
	if (state != State::non) {
		func_[int(state)]();
	}

	ObjectUpdate();

	for (auto& i : instanceObject) {
		if (i->GetInstanceDrawNum() <= 0.0f) { continue; }
		i->Update();
	}

	if (!isCollision) {
		hitTimer->Update();
		//判定を取らない時間以上になったら戻す
		if (*hitTimer.get() > 20) {
			isCollision = true;
		}
	}
}

void Boss1NearAttack1::Draw()
{
	for (auto& i : instanceObject) {
		if (i->GetInstanceDrawNum() <= 0.0f) { continue; }
		i->Draw();
	}
}

void Boss1NearAttack1::FrameReset()
{
	for (auto& i : instanceObject) {
		i->FrameReset();
	}
}

void Boss1NearAttack1::GetAttackCollisionBox(std::vector<Box>& _info)
{
	for (auto& i : object) {
		if (i.alpha < 0.3f) { continue; }
		Box add;
		add.point1 = { i.pos.x - 2.0f,i.pos.y - 10.0f,i.pos.z - 2.0f };
		add.point2 = { i.pos.x + 2.0f,i.pos.y + 10.0f,i.pos.z + 2.0f };
		_info.emplace_back(add);
	}
}

void Boss1NearAttack1::StartMove()
{
	if (!boss->GetBaseModel()->GetIsAnimationEnd()) { return; }
	state = State::attack;

}

void Boss1NearAttack1::Attack()
{
	timer->Update();

	//1秒立っていないなら出現させない
	if ((*timer.get() - oldtime) < 1.0f) {
		return;
	}
	
	oldtime = timer->GetTime();

	if (*timer.get() % 5 != 0) { return; }
	//出現数
	XMFLOAT3 pos = boss->GetCenter()->GetPosition();
	float inNum = timer->GetTime();
	if (inNum >= 40.0f) {
		inNum = 40.0f;
	}
	const float addAngle = 360.0f / inNum;
	float angle = timer->GetTime() * 2.0f;
	for (int i = 0; i < int(inNum); i++) {
		//リストに要素を追加
		object.emplace_front();
		//追加した要素の参照
		ObjectInfo& add = object.front();
		//値のリセット
		add.isUp = true;
		add.timer = std::make_unique<Engine::Timer>();
		float radian = XMConvertToRadians(angle);
		add.pos.x = pos.x + dist * sinf(radian);
		add.pos.y = 0.0f;
		add.pos.z = pos.z + dist * cosf(radian);
		add.alpha = 1.0f;

		angle += addAngle;
	}
	
	dist += 3.0f;

	const float maxTimer = 100.0f;
	if (*timer.get() > maxTimer - 5.0f) {
		boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::nearAttak_end));
	}

	if (*timer.get() < maxTimer) { return; }
	state = State::finish;

}

void Boss1NearAttack1::Finish()
{
	int size = int(std::distance(object.begin(), object.end()));
	if (size != 0) { return; }
	isEnd = true;
}

void Boss1NearAttack1::ObjectUpdate()
{
	//全更新
	for (std::forward_list<ObjectInfo>::iterator it = object.begin();
		it != object.end(); it++) {
		//経過フレーム数をカウント
		it->timer->Update();

		//上昇
		const float maxTimer = 10.0f;
		float rate = *it->timer.get() / maxTimer;
		if (rate < 1.0) {
			it->pos.y = Easing::Lerp(0.0f, 5.0f, rate);
		}
		else {
			it->alpha -= 0.05f;
		}

		if (it->alpha > 0.0f) {
			for (auto& i : instanceObject) {
				if (!i->GetInstanceDrawCheck()) { continue; }
				i->DrawInstance(it->pos, { 1.0f,5.0f,1.0f }, { 1.0f,1.0f,1.0f }, { 1,1,1,it->alpha });
			}
		}
	}

	//攻撃削除
	object.remove_if([](ObjectInfo& x) {
		return x.alpha <= 0.0f;
		}
	);
}
