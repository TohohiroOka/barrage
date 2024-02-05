#pragma once
#include "camera/CutSceneCamera.h"
#include "object/2d/Sprite.h"

#include <memory>

//�o�ꉉ�o
//�{�X�ɂ���ăJ�������[�N�⃊�\�[�X���قȂ�̂Ŋ��N���X
class BaseOnStageDirection
{
public:
	BaseOnStageDirection() = default;
	virtual ~BaseOnStageDirection() = default;

	virtual void Init() {};

	virtual void Update() {};

	virtual void Draw3D() {};

	virtual void Draw2D() {};

	virtual void ImguiDraw();

protected:
	//�ヌ�^�[�{�b�N�X
	std::unique_ptr<Sprite> letterBox;
	//�������^�[�{�b�N�X
	std::unique_ptr<Sprite> letterBoxUnder;
	//����
	std::unique_ptr<Sprite> firstName;
	std::unique_ptr<Sprite> secondName;
	//�J����
	std::unique_ptr<CutSceneCamera> camera;

};