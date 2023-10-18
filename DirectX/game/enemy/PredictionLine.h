#pragma once
#include "Object/3d/InstanceObject.h"

class PredictionLine
{
public:
	PredictionLine();
	~PredictionLine(){};

	void Update(const DirectX::XMFLOAT3& _start, const DirectX::XMFLOAT3& _end,
		const float _width, const DirectX::XMFLOAT4& _color = { 0.8f,0.0f,0.0f,0.5f });

	void Draw();

	void FrameReset();

private:

	//ƒ‚ƒfƒ‹
	std::unique_ptr<Model> model;
	std::unique_ptr<InstanceObject> object;
};

