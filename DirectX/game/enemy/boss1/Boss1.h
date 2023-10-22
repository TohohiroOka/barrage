#pragma once
#include "../BaseBoss.h"
#include "Object/3d/Object3d.h"

class Boss1 : public BaseBoss
{
private:
	
	//‰“‹——£s“®
	enum class LongAction {
		move1,
		middle,//ˆÚ“®‚ÆUŒ‚‚ÌŠÔ
		bullet1,
		bullet2,
		size,
	};

	//’†‹——£s“®
	enum class MediumAction {
		move1,
		middle,//ˆÚ“®‚ÆUŒ‚‚ÌŠÔ
		bullet3,
		size,
	};

	//‹ß‹——£s“®
	enum class ShortAction {
		move1,
		middle,//ˆÚ“®‚ÆUŒ‚‚ÌŠÔ
		attack1,
		others,
		move2,
		size,
	};

public:
	Boss1();
	~Boss1(){};

	void Update() override;

	void Draw() override;

	void FrameReset() override;

	void SetAction() override;

	void SetShortMoveAction();

private:
	std::unique_ptr<Model> model;
	static const int partsNum = 8;
	static const std::array<Vector3, partsNum> partsDist;
	std::array<std::unique_ptr<Object3d>, partsNum> boss;

};

