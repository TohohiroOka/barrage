#pragma once
#include "Object/3d/Object3d.h"
#include "Object/2d/Sprite.h"

#include <memory>


class LockonUI
{
public:
	LockonUI();
	~LockonUI() = default;

	void Init(Camera *camera);

	void Update();

	void Draw();

	void StartLockOnAnimation(const DirectX::XMFLOAT2 *enemyPos);

	void EndLockOnDraw() { isDraw = false; }

private:
	std::unique_ptr<Model>		lockonModel;
	std::unique_ptr<Object3d>	lockOnObject;
	std::unique_ptr<Sprite>		lockonSprite;

	//カメラ参照
	Camera* camera;
	
	//敵座標（参照）
	const DirectX::XMFLOAT2 *position;

	//現フレーム
	int frame = 0;

	//サイズ変動時間
	const int SCALE_ANIM_FRAME = 30;
	//表示時間
	const int DISPLAY_FRAME = 60;

	//表示フラグ
	bool isDraw = false;


};