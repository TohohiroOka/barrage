#pragma once
#include "Scene/InterfaceScene.h"
#include "Math/Vector3.h"
#include "engine/camera/DebugCamera.h"
#include "player/Player.h"
#include "game/camera/GameCamera.h"
#include "engine/Camera/LightCamera.h"
#include "../enemy/boss1/Boss1.h"
#include "../field/Field.h"
#include "../field/ShadowGround.h"

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
	///	解放
	/// </summary>
	void Finalize() override;

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
	bool isNormalCamera = false;
	float worldTimer;

	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<GameCamera> camera;
	std::unique_ptr<LightCamera> lightCamera;

	std::unique_ptr<Sprite> sprite;

	std::unique_ptr<Player> player;

	//地形
	std::unique_ptr<Field> field;

	//ボス
	std::unique_ptr<Boss1>boss;

	std::unique_ptr<FbxModel> fbxModel1;
	std::unique_ptr<FbxModel> fbxModel2;
	std::unique_ptr<Fbx> fbx;

	bool isBlend;
	float rate;

	std::unique_ptr<ShadowGround> ground;
};