#pragma once
#include "Object/3d/InstanceObject.h"

class BackGround
{
private:

	struct Info {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 rota;
	};

public:
	
	BackGround();
	~BackGround(){};

	void Update(const DirectX::XMFLOAT3& _playerPos);

	void Draw();

	void FrameReset();

private:

	std::unique_ptr<Model> model;
	std::array<std::unique_ptr<InstanceObject>, 4> object;
	std::vector<Info> info;

};

