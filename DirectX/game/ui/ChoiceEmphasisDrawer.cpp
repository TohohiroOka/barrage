#include "ChoiceEmphasisDrawer.h"

void ChoiceEmphasisDrawer::Initialize()
{
	TextureManager::LoadTexture("emphasis", "Resources/SpriteTexture/selectgradation.png");
	emphasisSprite = Sprite::Create("emphasis", {}, { 384.f,96.f });
	emphasisSprite->SetTexSize({ 384.f,96.f });
	emphasisSprite->SetAnchorpoint({ 0.5f,0.5f });
	emphasisSprite->Update();
}

void ChoiceEmphasisDrawer::Update()
{
	static bool isAdd = true;
	static int frame = 0;
	if (frame > ALPHA_ANIM_FRAME) {
		isAdd = false;
		frame = ALPHA_ANIM_FRAME;
	}
	if (frame < 0) {
		isAdd = true;
		frame = 0;
	}
	if (isAdd) { frame++; }
	else { frame--; }
	float rate = float(frame) / float(ALPHA_ANIM_FRAME);

	//1F前
	isChooseOld = isChoose;
	//決定時アニメーション
	if (isChoose) {
		choiceAnimFrame++;
		if (choiceAnimFrame > CHOICE_ANIM_FRAME) {
			choiceAnimFrame = 0;
			ALPHA_ANIM_FRAME = 90;
			isChoose = false;
		}
	}

	emphasisSprite->SetColor({ 1.f,1.f,1.f,rate + 0.2f });
	emphasisSprite->Update();
}

void ChoiceEmphasisDrawer::Draw()
{
	emphasisSprite->Draw();
}

void ChoiceEmphasisDrawer::SetEmphasisPos(float posX, float posY, float sizeX, float sizeY)
{
	emphasisSprite->SetPosition({ posX,posY });
	emphasisSprite->SetSize({ sizeX,sizeY });
}

void ChoiceEmphasisDrawer::PlayChoiseAnimation()
{
	ALPHA_ANIM_FRAME = 10;
	isChoose = true;
}

bool ChoiceEmphasisDrawer::IsChooseAnimEnd()
{
	return isChoose == false && isChooseOld == true;
}