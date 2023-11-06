#include "Boss1NearAttack1.h"
#include "../BaseBoss.h"
#include "GameHelper.h"
#include "../Math/Easing/Easing.h"

using namespace DirectX;

Boss1NearAttack1::Boss1NearAttack1()
{
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(1);
	model = Model::CreateFromOBJ("cone");
	for (auto& i : instanceObject) {
		i = InstanceObject::Create(model.get());
	}
	state = State::start;

	timer = std::make_unique<Engine::Timer>();
	isEnd = false;

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

void Boss1NearAttack1::GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info)
{
	for (auto& i : object) {
		BaseAction::AttackCollision add;
		add.pos = i.pos;
		add.radius = 1.0f;
		_info.emplace_back(add);
	}
}

void Boss1NearAttack1::StartMove()
{
	state = State::attack;
}

void Boss1NearAttack1::Attack()
{
	timer->Update();
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
