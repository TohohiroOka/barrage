#pragma once
#include "engine/Object/2d/Sprite.h"

//選択肢強調表示用クラス
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
	//強調用ぼかし
	std::unique_ptr<Sprite> emphasisSprite;

	//時間
	int ALPHA_ANIM_FRAME = 90;

	bool isChoose = false;
	bool isChooseOld = false;

	const int CHOICE_ANIM_FRAME = 60;
	int choiceAnimFrame = 0;
};