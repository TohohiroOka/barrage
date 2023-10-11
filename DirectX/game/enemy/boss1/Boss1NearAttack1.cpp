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
	isEnd = false;

	func_.emplace_back([this] {return StartMove(); });
	func_.emplace_back([this] {return BeforeMove(); });
	func_.emplace_back([this] {return Attack(); });
	func_.emplace_back([this] {return afterMove(); });
	func_.emplace_back([this] {return EndMove(); });
}

void Boss1NearAttack1::Update()
{
	//動き
	if (state != State::non) {
		func_[int(state)]();
	}

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

		//時間になったら更新
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
	const float maxTimer = 50.0f;
	const float rotationNum = 3.0f;
	
	//回転
	allRota = Easing::Lerp(0.0f, 360.0f * rotationNum, timer / maxTimer);
	boss->GetCenter()->SetRotation({ 0,allRota,0 });

	//個々にずらし
	for (int i = 0; i < objectNum; i++) {
		//ずらし
		if (timer > i * 2 && object[i].stateInState == 0) {
			float posx = Easing::Lerp(0.0f, 15.0f + i * 5, object[i].timer / (maxTimer / 5.0f));
			object[i].object->SetPosition({ object[i].pos.x + posx,object[i].pos.y ,object[i].pos.z });
			object[i].timer++;
			if (object[i].timer < maxTimer / 5) { continue; }
			object[i].timer = 0.0f;
			object[i].stateInState++;
		}
		//戻し
		else if (timer > maxTimer - (maxTimer / 5.0f) && object[i].stateInState == 1) {
			float posx = Easing::Lerp(15.0f + i * 5, 0.0f, object[i].timer / (maxTimer / 5.0f));
			object[i].object->SetPosition({ object[i].pos.x + posx,object[i].pos.y ,object[i].pos.z });
			object[i].timer++;
			if (object[i].timer < maxTimer / 5) { continue; }
			object[i].timer = 0.0f;
			object[i].stateInState++;
		}
		//ずれ中
		else if (object[i].stateInState == 1) {
			object[i].object->SetPosition({ object[i].pos.x + 15.0f + i * 5,object[i].pos.y ,object[i].pos.z });
		}
	}
	timer++;

	if (timer <= maxTimer + 20) { return; }
	timer = 0.0f;
	state = State::afterMove;
	for (auto& i : object) {
		i.stateInState = 0;
		i.pos = i.object->GetPosition();
	}
}

void Boss1NearAttack1::afterMove()
{
	const float maxTimer = 30.0f;

	XMFLOAT3 pos = {};
	const XMFLOAT3 centerPos = boss->GetCenter()->GetPosition();
	for (int i = 0; i < objectNum; i++) {
		pos.x = Easing::Lerp(object[i].pos.x, 0.0f, timer / maxTimer);
		pos.y = Easing::Lerp(object[i].pos.y, i * dist, timer / maxTimer);
		pos.z = Easing::Lerp(object[i].pos.z, 0.0f, timer / maxTimer);
		object[i].object->SetPosition(pos);
	}
	timer++;

	if (timer <= maxTimer) { return; }
	timer = 0.0f;
	state = State::end;
	int num = 0;
	for (auto& i : object) {
		i.pos = i.object->GetPosition();
		i.hokanPointNum = num % 9;
		num++;
	}
}

void Boss1NearAttack1::EndMove()
{
	const float maxTimer = 100.0f;

	XMFLOAT3 pos = boss->GetCenter()->GetPosition();

	const float maxHokanTimer = 15.0f;
	const std::vector<float> hokanPos = { 0.0f,10.0f,20.0f,10.0f,0.0,-10.0f,-20.0f,-10.0f,0.0f };

	for (int i = 0; i < objectNum; i++) {
		//y軸移動
		float posy = Easing::Lerp(object[i].pos.y, (pos.y - dist * (objectNum - 1 - i)) - 100.0f, timer / maxTimer);

		//x軸移動
		float posx = GameHelper::Instance()->SplinePosition(hokanPos, object[i].hokanPointNum, object[i].timer / maxHokanTimer);
		object[i].timer++;

		object[i].object->SetPosition({ object[i].pos.x + posx,posy,object[i].pos.z });

		//時間になったら更新
		if (object[i].timer <= maxHokanTimer) { continue; }
		if (object[i].hokanPointNum < hokanPos.size() - 1) {
			object[i].hokanPointNum++;
			object[i].timer = 0.0f;
		} else {
			object[i].hokanPointNum = 0;
			object[i].timer = 0.0f;
		}
	}

	timer++;

	if (timer <= maxTimer) { return; }
	timer = 0.0f;
	state = State::non;
	isEnd = true;
	for (auto& i : object) {
		i.pos = i.object->GetPosition();
		i.timer = 0.0f;
		i.hokanPointNum = 0;
	}
}
