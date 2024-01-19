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

	void Update(const DirectX::XMFLOAT3 &enemyPos);

	void Draw();

	void StartLockOnAnimation();

	void EndLockOnDraw() { isDraw = false; }

private:
	std::unique_ptr<Model>		lockonModel;
	std::unique_ptr<Object3d>	lockOnObject;
	std::unique_ptr<Sprite>		lockonSprite;

	//カメラ参照
	Camera* camera;

	//モデルの中心に合わせるためのy座標オフセット
	const float MODEL_Y_OFFSET = 8.f;

	//現フレーム
	int frame = 0;

	//サイズ変動時間
	const int SCALE_ANIM_FRAME = 30;
	//表示時間
	const int DISPLAY_FRAME = 60;

	//表示フラグ
	bool isDraw = false;


};