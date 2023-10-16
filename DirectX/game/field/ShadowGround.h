#pragma once
#include "Object/3d/Object3d.h"

class ShadowGround
{
private:

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	ShadowGround();
	~ShadowGround() {};

	void Update();
	void Draw();
	void DrawLightView();

private:
	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Object3d> object;
};