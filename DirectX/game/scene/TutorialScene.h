#pragma once
#include "Scene/InterfaceScene.h"
#include "engine/camera/DebugCamera.h"
#include "engine/Camera/LightCamera.h"

#include "system/ActionInputConfig.h"
#include "camera/TitleCamera.h"
#include "player/Player.h"
#include "field/Field.h"

#include <memory>

class TutorialScene : public InterfaceScene
{
private:// エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMINT3 = DirectX::XMINT3;

private: //enum
	/// <summary>
	/// チュートリアルの流れフェーズ
	/// </summary>
	enum class TutorialPhase
	{
		JUMP_
	};

public:

	TutorialScene() = default;
	~TutorialScene();

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
	/// 毎フレームリセット
	/// </summary>
	void FrameReset() override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	void CollisionCheck();

private:
	//カメラ
	bool isNormalCamera = true;
	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<TitleCamera> camera;
	std::unique_ptr<LightCamera> lightCamera;

	//スプライト

	//地形
	std::unique_ptr<Field> field;
	//プレイヤー
	std::unique_ptr<Player> player;

	//行動入力設定
	std::unique_ptr<ActionInputConfig> actionInputConfig;
	bool isInputConfigMode = false;

	//シーン遷移待機中か
	bool isSceneChangeWait = false;
};

