#pragma once
#include "Scene/InterfaceScene.h"
#include "Math/Vector3.h"
#include "player/Player.h"
#include "game/camera/GameCamera.h"
#include "engine/Camera/LightCamera.h"
#include "../enemy/boss1/Boss1.h"
#include "../field/Field.h"
#include "game/ui/GameOver.h"
#include "system/ActionInputConfigBase.h"
#include "game/cutscene/Boss1Defeat.h"
#include "ui/LockonUI.h"
#include "../game/effect/ScreenCut.h"
#include "PauseScene.h"

class Scene1 : public InterfaceScene
{
private:// エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMINT3 = DirectX::XMINT3;

public:

	Scene1(){};
	~Scene1() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	///	更新
	/// </summary>
	void Update()override;

	/// <summary>
	///	描画
	/// </summary>
	/// <param name="_cameraNum">カメラ番号</param>
	void Draw(const int _cameraNum) override;

	/// <summary>
	///	描画
	/// </summary>
	/// <param name="_cameraNum">カメラ番号</param>
	void DrawLightView(const int _cameraNum) override;

	/// <summary>
	/// ポストエフェクトをかけない描画
	/// </summary>
	void NonPostEffectDraw(const int _cameraNum) override;

	/// <summary>
	/// imguiの表示
	/// </summary>
	void ImguiDraw() override;

	/// <summary>
	/// 
	/// </summary>
	void FrameReset() override;

	void CollisionCheck();

private:
	std::unique_ptr<GameCamera> camera;
	std::unique_ptr<LightCamera> lightCamera;

	std::unique_ptr<Sprite> sprite;

	std::unique_ptr<Player> player;

	std::unique_ptr<ScreenCut> screenCut;

	//地形
	std::unique_ptr<Field> field;

	//ボス
	std::unique_ptr<Boss1>boss;
	//ゲームオーバーUI
	std::unique_ptr<GameOver> gameoverUi;

	//死亡演出
	std::unique_ptr<Boss1Defeat> defeatDirection;

	//ロックオン
	std::unique_ptr<LockonUI> lockonUI;

	//ヒットストップフレーム数
	int hitStopFrame = 5;

	//シーン遷移待機中か
	bool isSceneChangeWait = false;

	//ポースシーン管理
	std::unique_ptr<PauseScene> pauseScene;

};