#pragma once
#include "Scene/InterfaceScene.h"
#include "engine/Camera/LightCamera.h"

#include "ui/ChoiceEmphasisDrawer.h"
#include "system/ActionInputConfigKey.h"
#include "system/ActionInputConfigPad.h"
#include "camera/TitleCamera.h"
#include "player/Player.h"
#include "field/Field.h"
#include "titleObject/Portal.h"
#include "titleObject/SpeechBubble.h"
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

	/// <summary>
	/// ポータルに入るかチェック
	/// </summary>
	void IntoPortalCheck();

	/// <summary>
	/// ポータルに入る行動を開始
	/// </summary>
	void IntoPortalStart();

	/// <summary>
	/// ポータルに入る行動
	/// </summary>
	void IntoPortal();

private:
	//カメラ
	std::unique_ptr<TitleCamera> camera;
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
	//ポータル説明用吹き出しオブジェクト
	std::array<std::unique_ptr<SpeechBubble>, 3> speechBubbles;

	//行動入力設定
	std::unique_ptr<ActionInputConfigKey> actionInputConfigKey;
	std::unique_ptr<ActionInputConfigPad> actionInputConfigPad;
	bool isInputConfigKey = false;
	bool isInputConfigPad = false;

	//選択ボタンが押下可能UI
	std::unique_ptr<PressSelectButtonUI> pressSelectButtonUI;

	//入るポータル
	Portal* selectPortal = nullptr;
	//ポータルに入る行動をしているか
	bool isIntoPortal = false;

	//シーン遷移待機中か
	bool isSceneChangeWait = false;
};

