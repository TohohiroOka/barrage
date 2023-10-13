#include "PredictionLine.h"
#include "GameHelper.h"

PredictionLine::PredictionLine()
{
	model = Model::CreateFromOBJ("NormalCube");
	object = Object3d::Create(model.get());
	object->SetColor({ 0.8f,0.0f,0.0f,1.0f });
}

void PredictionLine::Update(const DirectX::XMFLOAT3& _start, const DirectX::XMFLOAT3& _end, const float _width)
{
	Vector3 moveVec = Vector3(_end- _start);
	Vector3 vec = GameHelper::Instance()->VelocityRotate(moveVec);
	object->SetRotation(vec);
	float length = moveVec.length();
	object->SetScale({ _width ,_width,length });
	object->SetPosition({ _start + DirectX::XMFLOAT3(moveVec) });
}

void PredictionLine::Draw()
{
	object->Draw();
}
