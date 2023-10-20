#include "PredictionLine.h"
#include "GameHelper.h"

PredictionLine::PredictionLine()
{
	model = Model::CreateFromOBJ("NormalCube");
	for (auto& i : object) {
		i = InstanceObject::Create(model.get());
	}
}

void PredictionLine::Update(const DirectX::XMFLOAT3& _start, const DirectX::XMFLOAT3& _end,
	const float _width, const DirectX::XMFLOAT4& _color)
{
	Vector3 moveVec = Vector3(_end - _start);
	Vector3 vec = VelocityRotate(moveVec);
	float length = moveVec.length();

	for (auto& i : object) {
		if (!i->GetInstanceDrawCheck()) { continue; }
		i->DrawInstance({ _start + DirectX::XMFLOAT3(moveVec) }, { _width ,_width,length }, vec, _color);
	}
}

void PredictionLine::Draw()
{
	for (auto& i : object) {
		if (i->GetInstanceDrawNum() == 0) { continue; }
		i->Draw();
	}
}

void PredictionLine::FrameReset()
{
	for (auto& i : object) {
		if (i->GetInstanceDrawNum() == 0) { continue; }
		i->FrameReset();
	}
}
