#include "BaseBullet.h"

std::unique_ptr<InstanceObject> BaseBullet::instanceObject;
std::unique_ptr<PredictionLine> BaseBullet::predictionLine;

BaseBullet::BaseBullet(const Vector3& _pos, const Vector3& _scale, const Vector3& _rota, const Vector3& _moveVec)
{
	isAlive = true;
	pos = _pos;
	scale = _scale;
	rota = _rota;
	moveVec = _moveVec;

	timer = 0;
}

void BaseBullet::Update()
{
	pos += moveVec;

	const float dist = 10;
	if (pos.x < -dist || pos.x > 510.0f + dist ||
		pos.y < -dist || pos.y > 255 + dist ||
		pos.z < -dist || pos.z > 510.0f + dist) {
		isAlive = false;
		return;
	}

	timer++;

	if (!instanceObject->GetInstanceDrawCheck()) { return; }
	instanceObject->DrawInstance(pos, scale, rota, { 1,1,1,1 });

	Vector3 endPoint={};
	if (timer < 20.0f) {
		endPoint = pos - (moveVec * timer / 2.0f);
	} else {
		endPoint = pos - (moveVec * 10.0f);
	}
	predictionLine->Update(pos, endPoint, 1.0f, { 1.0f,1.0f,1.0f,0.5f });
}

void BaseBullet::Draw()
{
}

void BaseBullet::StaticDraw()
{
	predictionLine->Draw();
	instanceObject->Draw();
}

void BaseBullet::FrameReset()
{
	instanceObject->FrameReset();
	predictionLine->FrameReset();
}

void BaseBullet::Reset()
{
	instanceObject.reset();
}

void BaseBullet::CreateInstance(Model* _model)
{
	instanceObject = InstanceObject::Create(_model);
	predictionLine = std::make_unique<PredictionLine>();
}
