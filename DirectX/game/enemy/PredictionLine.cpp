#include "PredictionLine.h"
#include "GameHelper.h"

PredictionLine::PredictionLine()
{
	model = Model::CreateFromOBJ("NormalCube");
	object = InstanceObject::Create(model.get());
}

void PredictionLine::Update(const DirectX::XMFLOAT3& _start, const DirectX::XMFLOAT3& _end,
	const float _width, const DirectX::XMFLOAT4& _color)
{
	Vector3 moveVec = Vector3(_end- _start);
	Vector3 vec = GameHelper::Instance()->VelocityRotate(moveVec);
	float length = moveVec.length();

	object->DrawInstance({ _start + DirectX::XMFLOAT3(moveVec) }, { _width ,_width,length }, vec, _color);
}

void PredictionLine::Draw()
{
	object->Draw();
}

void PredictionLine::FrameReset()
{
	object->FrameReset();
}
