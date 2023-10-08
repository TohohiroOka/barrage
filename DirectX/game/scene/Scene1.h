#pragma once
#include "Scene/InterfaceScene.h"
#include "Math/Vector3.h"
#include "player/Player.h"
#include "game/camera/GameCamera.h"
#include "../enemy/boss1/Boss1.h"

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

private:

	std::unique_ptr<GameCamera> camera;

	std::unique_ptr<Sprite> sprite;

	std::shared_ptr<Player> player;

	//モデル
	std::shared_ptr<TerrainModel> gmodel;
	//オブジェクト
	std::shared_ptr<HeightMap> gobject;

	std::unique_ptr<Boss1>boss;
};