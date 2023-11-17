#pragma once
#include "Scene/InterfaceScene.h"
#include "cutScene/Boss1OnStage.h"
#include "engine/Camera/LightCamera.h"
#include "../field/Field.h"
#include "camera/DebugCamera.h"

class OnStageTestScene : public InterfaceScene
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
	//地形
	std::unique_ptr<Field> field;

	std::unique_ptr<LightCamera> lightCamera;
	std::unique_ptr<DebugCamera> debugCamera;

	Boss1OnStage onStageDirection;

	bool isNormalCamera = true;
};
