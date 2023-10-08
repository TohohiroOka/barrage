#include "Boss1NearAttack1.h"
#include "../BaseBoss.h"
#include "GameHelper.h"
#include "../Math/Easing/Easing.h"

using namespace DirectX;
const float maxScale = 10.0f;
const float dist = maxScale + 5.0f;

Boss1NearAttack1::Boss1NearAttack1()
{
	model = Model::CreateFromOBJ("NormalCube");
	XMFLOAT3 pos = boss->GetCenter()->GetPosition();
	for (int i = 0; i < objectNum; i++) {
		object[i].pos = { 0.0,pos.y - dist * (objectNum - 1 - i) ,0.0 };
		object[i].object = Object3d::Create(model.get());
		object[i].object->SetPosition(object[i].pos);
		object[i].object->SetScale({ maxScale,maxScale,maxScale });
		object[i].object->SetParent(boss->GetCenter());
		object[i].timer = 0.0f;
		object[i].hokanPointNum = i % 9;
	}

	state = State::start;

	timer = 0.0f;

	func_.emplace_back([this] {return StartMove(); });
	func_.emplace_back([this] {return BeforeMove(); });
	func_.emplace_back([this] {return Attack(); });
	func_.emplace_back([this] {return afterMove(); });
	func_.emplace_back([this] {return EndMove(); });
}

void Boss1NearAttack1::Update()
{
	//ìÆÇ´
	func_[int(state)]();

	for (auto& i : object) {
		i.object->Update();
	}
}

void Boss1NearAttack1::Draw()
{
	for (auto& i : object) {
		i.object->Draw();
	}
}

bool Boss1NearAttack1::End()
{
	return false;
}

void Boss1NearAttack1::StartMove()
{
	const float maxTimer = 100.0f;
	const float maxY = 100.0f;
	float posy = Easing::Lerp(0, maxY, timer / maxTimer);

	const float maxHokanTimer = 15.0f;
	const std::vector<float> hokanPos = { 0.0f,10.0f,20.0f,10.0f,0.0,-10.0f,-20.0f,-10.0f,0.0f };

	for (auto& i : object) {
		float posx = GameHelper::Instance()->SplinePosition(hokanPos, i.hokanPointNum, i.timer / maxHokanTimer);
		i.timer++;

		i.object->SetPosition({ i.pos.x + posx,i.pos.y + posy,i.pos.z });

		//éûä‘Ç…Ç»Ç¡ÇΩÇÁçXêV
		if (i.timer <= maxHokanTimer) { continue; }
		if (i.hokanPointNum < hokanPos.size() - 1) {
			i.hokanPointNum++;
			i.timer = 0.0f;
		} else {
			i.hokanPointNum = 0;
			i.timer = 0.0f;
		}
	}

	timer++;

	if (timer <= maxTimer) { return; }
	timer = 0.0f;
	state = State::beforeMove;
	for (auto& i : object) {
		i.pos = i.object->GetPosition();
		i.timer = 0.0f;
		i.hokanPointNum = 0;
	}
}

void Boss1NearAttack1::BeforeMove()
{
	const float maxTimer = 30.0f;

	XMFLOAT3 pos = {};
	const XMFLOAT3 centerPos = boss->GetCenter()->GetPosition();
	for (int i = 0; i < objectNum; i++) {
		pos.x = Easing::Lerp(object[i].pos.x, 0.0f, timer / maxTimer);
		pos.y = Easing::Lerp(object[i].pos.y, 5.0f, timer / maxTimer);
		pos.z = Easing::Lerp(object[i].pos.z, i * dist, timer / maxTimer);
		object[i].object->SetPosition(pos);
	}
	timer++;

	if (timer <= maxTimer) { return; }
	timer = 0.0f;
	state = State::attack;
	for (auto& i : object) {
		i.pos = i.object->GetPosition();
	}
}

void Boss1NearAttack1::Attack()
{
	const float maxTimer = 500.0f;
	const float rotationNum = 3.0f;
	
	//âÒì]
	allRota = Easing::Lerp(0.0f, 360.0f * rotationNum, timer / maxTimer);
	boss->GetCenter()->SetRotation({ 0,allRota,0 });

	const std::vector<float> hokanPos = { 0.0f,10.0f,20.0f,10.0f,0.0f,0.0f,0.0f };

	//å¬ÅXÇ…Ç∏ÇÁÇµ
	for (int i = 0; i < objectNum; i++) {
		if (timer > i * 2 && object[i].hokanPointNum < 5) {
			float posx = GameHelper::Instance()->SplinePosition(hokanPos, object[i].hokanPointNum, object[i].timer / (maxTimer / 5.0f));
			object[i].object->SetPosition({ object[i].pos.x + posx,object[i].pos.y ,object[i].pos.z });
			object[i].timer++;
			if (object[i].timer < maxTimer / 5) { continue; }
			object[i].timer = 0.0f;
			object[i].hokanPointNum++;
		}
	}
	timer++;

	if (timer <= maxTimer + 20) { return; }
	timer = 0.0f;
	state = State::afterMove;
	for (auto& i : object) {
		i.pos = i.object->GetPosition();
	}
}

void Boss1NearAttack1::afterMove()
{
}

void Boss1NearAttack1::EndMove()
{
}
