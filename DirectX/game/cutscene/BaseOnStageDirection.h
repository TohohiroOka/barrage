#pragma once
#include "camera/CutSceneCamera.h"
#include "object/2d/Sprite.h"

#include <memory>

//登場演出
//ボスによってカメラワークやリソースが異なるので基底クラス
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
	//上レターボックス
	std::unique_ptr<Sprite> letterBox;
	//下側レターボックス
	std::unique_ptr<Sprite> letterBoxUnder;
	//文字
	std::unique_ptr<Sprite> firstName;
	std::unique_ptr<Sprite> secondName;
	//カメラ
	std::unique_ptr<CutSceneCamera> camera;

};