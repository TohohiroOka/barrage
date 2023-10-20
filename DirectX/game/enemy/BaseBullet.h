#pragma once
#include "BaseAction.h"
#include "Object/3d/InstanceObject.h"
#include "../game/enemy/PredictionLine.h"
#include "Math/Vector3.h"
#include <forward_list>

class BaseBullet : public BaseAction
{
public:

	virtual ~BaseBullet() {};
	
	virtual void Update() override;

	void Draw() override;

	void FrameReset() override;

	virtual void GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info) override{};

protected:
	static const int instanceNum = 5;
	//�e�`��p
	std::array<std::unique_ptr<InstanceObject>, instanceNum> instanceObject;
	//�e���`��p
	std::unique_ptr<PredictionLine> predictionLine;

	//�^�C�}�[
	float timer;
	//���f��
	std::unique_ptr<Model> model;
};