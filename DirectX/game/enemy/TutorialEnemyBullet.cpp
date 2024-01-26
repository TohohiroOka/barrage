#include "TutorialEnemyBullet.h"

const DirectX::XMFLOAT3 TutorialEnemyBullet::moveMaxPos = {
GameHelper::Instance()->GetStageSize(),GameHelper::Instance()->GetStageSize(),GameHelper::Instance()->GetStageSize() };
std::unique_ptr<Model> TutorialEnemyBullet::bulletModel;
std::unique_ptr<InstanceObject> TutorialEnemyBullet::object;
std::unique_ptr<PredictionLine> TutorialEnemyBullet::predictionLine;

TutorialEnemyBullet::TutorialEnemyBullet(const Vector3& _pos, const Vector3& _moveVec)
{
	isAlive = true;
	pos = _pos;
	moveVec = _moveVec;
}

void TutorialEnemyBullet::StaticInitialize()
{
	//’e“¹•`‰æ—p
	predictionLine = std::make_unique<PredictionLine>();

	//’e•`‰æ—p
	bulletModel = Model::CreateFromOBJ("bullet");
	object = InstanceObject::Create(bulletModel.get());
}

void TutorialEnemyBullet::Update()
{
	oldPos = pos;
	pos += moveVec;

	if (pos.x<-10.0f || pos.x>moveMaxPos.x + 10.0f ||
		pos.y<-10.0f || pos.y>moveMaxPos.y + 10.0f ||
		pos.z<-10.0f || pos.z>moveMaxPos.z + 10.0f) {
		isAlive = false;
	}

	object->DrawInstance(pos, { 1.0f,1.0f,1.0f }, {}, { 1.0f,1.0f,1.0f,1.0f });

	//’e“¹
	predictionLine->AddLine(pos, oldPos, 1.0f, { 0.5f,0.5f,0.5f,0.5f });
}

void TutorialEnemyBullet::Draw()
{
	object->Update();
	object->Draw();

	predictionLine->Update();
	predictionLine->Draw();
}

void TutorialEnemyBullet::FrameReset()
{
	if (object) {
		object->FrameReset();
	}
	if (predictionLine) {
		predictionLine->FrameReset();
	}
}