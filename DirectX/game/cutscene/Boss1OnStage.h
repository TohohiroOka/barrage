#pragma once

#include "BaseOnStageDirection.h"
#include "Object/3d/Fbx.h"
#include "Object/3d/FbxModel.h"
#include "enemy/boss1/Boss1Model.h"
#include "engine/Audio/Audio.h"

class Boss1OnStage : public BaseOnStageDirection
{
public:
	Boss1OnStage() = default;
	~Boss1OnStage() = default;


	void Init() override;

	void Update() override;

	void Draw3D() override;

	void Draw2D() override;

	void SetBase3DCamera();

	void StartDirection();

	bool GetIsDirectEnd() { return camera->GetIsCameraMoveEnd(); }

	void Reset();

	void FrameReset();

	void DrawLightView();

private:
	//オブジェクト座標
	DirectX::XMFLOAT3 playerObjectPos = { 100,7,20 };
	DirectX::XMFLOAT3 bossObjectPos = { 100,10,170 };

	//オブジェクト
	std::unique_ptr<Fbx> playerObject;
	std::unique_ptr<FbxModel> playerModel;
	std::unique_ptr<Boss1Model> boss1Model;

	//文字アニメーション用
	int frame = 0;
	//アニメーション再生時間
	const int TEXTANIM_MAXFRAME = 15;
	bool isDisplayBossText = false;

	
};