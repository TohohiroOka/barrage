#pragma once
#include "Scene/InterfaceScene.h"
#include "engine/camera/DebugCamera.h"
#include "engine/Camera/LightCamera.h"

#include "ui/ChoiceEmphasisDrawer.h"
#include "system/ActionInputConfig.h"
#include "camera/GameCamera.h"
#include "player/Player.h"
#include "field/Field.h"
#include "titleObject/Portal.h"
#include "ui/PressSelectButtonUI.h"

#include <memory>

class TitleScene : public InterfaceScene
{
private:// エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMINT3 = DirectX::XMINT3;

public:

	TitleScene() = default;
	~TitleScene();

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
	std::unique_ptr<GameCamera> camera;
	std::unique_ptr<LightCamera> lightCamera;

	//スプライト
	std::unique_ptr<Sprite> titleLogoSprite;	//タイトルロゴ
	std::unique_ptr<Sprite> intoPortalSprite;	//「入る」

	//地形
	std::unique_ptr<Field> field;
	//プレイヤー
	std::unique_ptr<Player> player;
	//シーン変更ポータル
	std::array<std::unique_ptr<Portal>, 3> portals;

	//行動入力設定
	std::unique_ptr<ActionInputConfig> actionInputConfig;
	bool isInputConfigMode = false;

	//選択ボタンが押下可能UI
	std::unique_ptr<PressSelectButtonUI> pressSelectButtonUI;
};

