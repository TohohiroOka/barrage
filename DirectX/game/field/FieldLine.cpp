#include "FieldLine.h"
#include "Math/Easing/Easing.h"
#include "GameHelper.h"

std::unique_ptr<PrimitiveObject3D> FieldLine::object;

FieldLine::FieldLine()
{
	spos = {};
	direction = MoveDirection(RandomInt(360) % int(MoveDirection::size));
	float size = GameHelper::Instance()->GetStageSize();
	if (direction == MoveDirection::x) {
		spos.x = RandomFloat(size * 5.0f) / 10.0f;
		spos.y = RandomFloat(size * 10.0f) / 10.0f;
		if (RandomSign() == 1) { spos.z = size; }
	}else if (direction == MoveDirection::x_) {
		spos.x = (RandomFloat(size * 5.0f) + size * 5.0f) / 10.0f;
		spos.y = RandomFloat(size * 10.0f) / 10.0f;
		if (RandomSign() == 1) { spos.z = size; }
	}else if (direction == MoveDirection::z) {
		if (RandomSign() == 1) { spos.x = size; }
		spos.y = RandomFloat(size * 10.0f) / 10.0f;
		spos.z = RandomFloat(size * 5.0f) / 10.0f;
	} else if (direction == MoveDirection::z_) {
		if (RandomSign() == 1) { spos.x = size; }
		spos.y = RandomFloat(size * 10.0f) / 10.0f;
		spos.z = (RandomFloat(size * 5.0f) + size * 5.0f) / 10.0f;
	}
	else if (direction == MoveDirection::y) {
		if (RandomSign() == 1) { 
			spos.x = (RandomInt(100) % 2) * size;
			spos.z = RandomFloat(size * 10.0f) / 10.0f;
		} else {
			spos.x = RandomFloat(size * 10.0f) / 10.0f;
			spos.z = (RandomInt(100) % 2) * size;
		}
		spos.y = RandomFloat(size * 5.0f) / 10.0f;
	} else if (direction == MoveDirection::y_) {
		if (RandomSign() == 1) {
			spos.x = (RandomInt(100) % 2) * size;
			spos.z = RandomFloat(size * 10.0f) / 10.0f;
		} else {
			spos.x = RandomFloat(size * 10.0f) / 10.0f;
			spos.z = (RandomInt(100) % 2) * size;
		}
		spos.y = (RandomFloat(size * 5.0f) + size * 5.0f) / 10.0f;
	}

	epos = spos;

	for (auto& i : timer) {
		i = std::make_unique<Engine::Timer>();
	}

	isEnd = false;
}

void FieldLine::StaticInitialize()
{
	object = PrimitiveObject3D::Create();
}

void FieldLine::Update()
{
	const float maxTime = 50;

	if (*timer[1].get() > maxTime) {
		isEnd = true;
		return;
	}

	timer[0]->Update();
	DirectX::XMFLOAT3 pos = spos;
	if (direction == MoveDirection::x) {
		pos.x = Easing::InExpo(spos.x, 600, *timer[0].get() / maxTime);
	}else if (direction == MoveDirection::x_) {
		pos.x = Easing::InExpo(spos.x, -600, *timer[0].get() / maxTime);
	}else if (direction == MoveDirection::y) {
		pos.y = Easing::InExpo(spos.y, 600, *timer[0].get() / maxTime);
	} else if (direction == MoveDirection::y_) {
		pos.y = Easing::InExpo(spos.y, -600, *timer[0].get() / maxTime);
	} else if (direction == MoveDirection::z) {
		pos.z = Easing::InExpo(spos.z, 600, *timer[0].get() / maxTime);
	} else if (direction == MoveDirection::z_) {
		pos.z = Easing::InExpo(spos.z, -600, *timer[0].get() / maxTime);
	}
	object->SetVertex(pos);

	//àÍíËà»è„Ç»ÇÁèIì_Çà⁄ìÆ
	if (*timer[0].get() < 10) {
		object->SetVertex(epos);
		return;
	}
	timer[1]->Update();
	pos = epos;
	if (direction == MoveDirection::x) {
		pos.x = Easing::InExpo(epos.x, 600, *timer[1].get() / maxTime);
	} else if (direction == MoveDirection::x_) {
		pos.x = Easing::InExpo(epos.x, -600, *timer[1].get() / maxTime);
	} else if (direction == MoveDirection::y) {
		pos.y = Easing::InExpo(epos.y, 600, *timer[1].get() / maxTime);
	} else if (direction == MoveDirection::y_) {
		pos.y = Easing::InExpo(epos.y, -600, *timer[1].get() / maxTime);
	} else if (direction == MoveDirection::z) {
		pos.z = Easing::InExpo(epos.z, 600, *timer[1].get() / maxTime);
	} else if (direction == MoveDirection::z_) {
		pos.z = Easing::InExpo(epos.z, -600, *timer[1].get() / maxTime);
	}
	object->SetVertex(pos);
}

void FieldLine::StaticUpdate()
{
	if (object->VertexNum() != 0) {
		object->VertexInit();
	}
}

void FieldLine::StaticDraw()
{
	if (object->VertexNum() != 0) {
		object->Draw();
	}
}
