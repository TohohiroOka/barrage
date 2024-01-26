#pragma once

#include "Object/2d/Sprite.h"

#include <array>
#include <memory>

class MessageBoxDrawer
{
public:

	MessageBoxDrawer() = default;
	~MessageBoxDrawer() = default;

	void Init();

	void Update();

	void Draw();

	void StartDisplay(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size,bool isCtrlPointLT = true);

	void EndDisplay();

private:
	//枠線
	std::array<std::unique_ptr<Sprite>, 4>	cornerSprites;
	std::array<std::unique_ptr<Sprite>, 2>	verticalSprites;
	std::array<std::unique_ptr<Sprite>, 2>	horizontalSprites;
	std::unique_ptr<Sprite>					backSprite;

	//メッセージウィンドウの情報
	DirectX::XMFLOAT2	size;			//ウィンドウサイズ
	DirectX::XMFLOAT2	pos;			//基準座標
	bool				isCtrlPointLT;	//基準点は左上か

	//描画用の画像の情報
	const float SPRITE_WIDTH = 32.f;
	const float SPRITE_HEIGHT = 32.f;
	

	//描画フラグ
	bool isDisplay = false;

	//初回生成時のロード
	static bool isLoadedResources;

};