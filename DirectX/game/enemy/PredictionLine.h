#pragma once
#include "Object/3d/Object3d.h"

class PredictionLine
{
public:
	PredictionLine();
	~PredictionLine(){};

	void Update(const DirectX::XMFLOAT3& _start, const DirectX::XMFLOAT3& _end, const float _width);

	void Draw();

private:

	//ƒ‚ƒfƒ‹
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;
};

