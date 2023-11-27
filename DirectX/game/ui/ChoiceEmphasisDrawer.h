#pragma once
#include "engine/Object/2d/Sprite.h"

//�I���������\���p�N���X
class ChoiceEmphasisDrawer
{
public:
	ChoiceEmphasisDrawer() = default;
	~ChoiceEmphasisDrawer() = default;

	void Initialize();

	void Update();

	void Draw();

	void SetEmphasisPos(float posX, float posY, float sizeX, float sizeY);

	void PlayChoiseAnimation();

	bool IsChooseAnimEnd();

private:
	//�����p�ڂ���
	std::unique_ptr<Sprite> emphasisSprite;

	//����
	int ALPHA_ANIM_FRAME = 90;

	bool isChoose = false;
	bool isChooseOld = false;

	const int CHOICE_ANIM_FRAME = 60;
	int choiceAnimFrame = 0;
};