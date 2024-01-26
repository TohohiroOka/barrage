#include "game/ui/MessageBoxDrawer.h"

#include "engine/Math/DirectXMathHelper.h"
#include "engine/Texture/TextureManager.h"

bool MessageBoxDrawer::isLoadedResources = false;

void MessageBoxDrawer::Init()
{
	if (!isLoadedResources) {
		TextureManager::LoadTexture("mb_corner", "Resources/SpriteTexture/messageBox/messageBox_corner.png");
		TextureManager::LoadTexture("mb_vt", "Resources/SpriteTexture/messageBox/messageBox_vertical.png");
		TextureManager::LoadTexture("mb_hl", "Resources/SpriteTexture/messageBox/messageBox_horizontal.png");
		TextureManager::LoadTexture("mb_back", "Resources/SpriteTexture/messageBox/messageBox_back.png");
		isLoadedResources = true;
	}

	size = { 512,128 };
	pos = { 256,64 };
	isCtrlPointLT = true;

	for (auto& s : cornerSprites) { if (!s) { s = Sprite::Create("mb_corner", {}, {}); } }
	for (auto& s : verticalSprites) { if (!s) { s = Sprite::Create("mb_vt", {}, { 0.5f,0.5f }); } }
	for (auto& s : horizontalSprites) { if (!s) { s = Sprite::Create("mb_hl", {}, { 0.5f,0.5f }); } }
	backSprite = Sprite::Create("mb_back", {}, { 0.5f,0.5f });

	//élã˜ÇÃç¿ïWåvéZóp
	float right = pos.x + (size.x / 2);
	float left = pos.x - (size.x / 2);
	float top = pos.y - (size.y / 2);
	float bottom = pos.y + (size.y / 2);
	//élã˜
	for (int i = 0; i < 4; i++) { cornerSprites[i]->SetRotation(90.f * i); }
	cornerSprites[0]->SetPosition({ left,top });
	cornerSprites[1]->SetPosition({ right,top });
	cornerSprites[2]->SetPosition({ right,bottom });
	cornerSprites[3]->SetPosition({ left,bottom });
	//èc
	for (int i = 0; i < 2; i++) { 
		verticalSprites[i]->SetRotation(180.f * i); 
		verticalSprites[i]->SetSize({ 32.f,size.y - SPRITE_HEIGHT });
	}
	verticalSprites[0]->SetPosition({ left + SPRITE_WIDTH / 2,pos.y });
	verticalSprites[1]->SetPosition({ right - SPRITE_WIDTH / 2,pos.y });
	//â°
	for (int i = 0; i < 2; i++) { 
		horizontalSprites[i]->SetRotation(180.f * i);
		horizontalSprites[i]->SetSize({ size.x - SPRITE_WIDTH,32.f });
	}
	horizontalSprites[0]->SetPosition({ pos.x,top + SPRITE_HEIGHT / 2 });
	horizontalSprites[1]->SetPosition({ pos.x,bottom - SPRITE_HEIGHT / 2 });
	//îwåi
	backSprite->SetPosition(pos);
	backSprite->SetSize(size);
	//çXêV
	for (auto& s : cornerSprites) { s->Update(); }
	for (auto& s : verticalSprites) { s->Update(); }
	for (auto& s : horizontalSprites) { s->Update(); }
	backSprite->Update();
}

void MessageBoxDrawer::Update()
{
	if (isDisplay) {
		for (auto& s : cornerSprites) { s->Update(); }
		for (auto& s : verticalSprites) { s->Update(); }
		for (auto& s : horizontalSprites) { s->Update(); }
		backSprite->Update();
	}
}

void MessageBoxDrawer::Draw()
{
	if (isDisplay) {
		backSprite->Draw();
		for (auto& s : cornerSprites) { s->Draw(); }
		for (auto& s : verticalSprites) { s->Draw(); }
		for (auto& s : horizontalSprites) { s->Draw(); }
	}
}

void MessageBoxDrawer::StartDisplay(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, bool isCtrlPointLT)
{
	this->size = size;
	this->pos = pos;

	isDisplay = true;
}

void MessageBoxDrawer::EndDisplay()
{
	isDisplay = false;
}
