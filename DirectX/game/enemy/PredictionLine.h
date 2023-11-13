#pragma once
#include "Object/3d/InstanceObject.h"

class PredictionLine
{
public:
	PredictionLine();
	~PredictionLine(){};

	void AddLine(const DirectX::XMFLOAT3& _start, const DirectX::XMFLOAT3& _end,
		const float _width, const DirectX::XMFLOAT4& _color = { 0.8f,0.0f,0.0f,0.5f });

	void Update();

	void Draw();

	void FrameReset();

private:

	//ƒ‚ƒfƒ‹
	std::unique_ptr<Model> model;
	static const int objectNum = 10;
	std::array<std::unique_ptr<InstanceObject>, objectNum> object;
};

