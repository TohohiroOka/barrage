#include "BackGround.h"
#include "GameHelper.h"
#include "Math/DirectXMathHelper.h"

BackGround::BackGround()
{
	using namespace DirectX;
	model= Model::CreateFromOBJ("NormalCube");
	for (auto& i : object) {
		i = InstanceObject::Create(model.get());
		i->SetBloom(true);
	}

	const float dist = 1000.0f;
	for (int i = 0; i < 1000; i++) {
		XMFLOAT3 radius = GetRandInUniformInCircle3D();
		Info add = {};
		add.pos = radius * (dist + RandomFloat(100.0f));
		add.scale = { 10.0f,10.0f,10.0f };
		info.emplace_back(add);
	}
}

void BackGround::Update(const DirectX::XMFLOAT3& _playerPos)
{
	int num = 0;
	for (auto& i : info) {
		for (int objectNum = num; objectNum < 4; objectNum++) {
			if (!object[objectNum]->GetInstanceDrawCheck()) {
				num++;
				continue;
			};
			object[objectNum]->DrawInstance(i.pos + _playerPos, i.scale, i.rota, { 1.0f,1.0f ,1.0f ,0.0f });
			break;
		}
	}

	for (auto& i : object) {
		if (i->GetInstanceDrawNum() == 0) { continue; }
		i->Update();
	}
}

void BackGround::Draw()
{
	for (auto& i : object) {
		i->Draw();
	}
}

void BackGround::FrameReset()
{
	for (auto& i : object) {
		i->FrameReset();
	}
}
