#pragma once
#include "camera/OnStageCamera.h"
#include "object/2d/Sprite.h"

#include <memory>

//登場演出
//ボスによってカメラワークやリソースが異なるので基底クラス
class BaseOnStageDirection
{
public:
	virtual void Init() {};

	virtual void Update() {};

	virtual void Draw() {};

	virtual void ImguiDraw();

protected:
	//上下の黒枠
	std::unique_ptr<Sprite> letterBox;
	//文字
	std::unique_ptr<Sprite> firstName;
	std::unique_ptr<Sprite> secondName;
	//カメラ
	OnStageCamera camera;

};