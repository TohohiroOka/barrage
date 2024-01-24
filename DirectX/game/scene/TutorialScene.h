#pragma once
#include "Scene/InterfaceScene.h"
#include "engine/camera/DebugCamera.h"
#include "engine/Camera/LightCamera.h"

#include "system/ActionInputConfig.h"
#include "camera/TutorialCamera.h"
#include "player/Player.h"
#include "field/Field.h"
#include "enemy/TutorialEnemy.h"

#include "ui/OKSprite.h"
#include "ui/LockonUI.h"

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
	/// チュートリアルのフェーズ
	/// </summary>
	enum class TutorialPhase
	{
		TUTORIAL_START,
		TUTORIAL_RUN,
		TUTORIAL_JUMP,
		TUTORIAL_CAMERA,
		TUTORIAL_ATTACK,
		TUTORIAL_AVOID,
		TUTORIAL_FREE,
	};

	/// <summary>
	/// チュートリアル(カメラ)のフェーズ
	/// </summary>
	enum class TutorialCameraPhasePhase
	{
		TUTORIAL_CAMERA_TEXT_1,
		TUTORIAL_CAMERA_ZOOM_ENEMY_BORN,
		TUTORIAL_CAMERA_ENEMY_BORN,
		TUTORIAL_CAMERA_RETURN_POS,
		TUTORIAL_CAMERA_TEXT_2_ACTION,
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

	//チュートリアルフェーズごとの更新内容
	void TutorialStartUpdate();
	void TutorialRunUpdate();
	void TutorialJumpUpdate();
	void TutorialCameraUpdate();
	void TutorialAttackUpdate();
	void TutorialAvoidUpdate();
	void TutorialFreeUpdate();

	//チュートリアルカメラ説明フェーズごとの更新処理
	void TutorialCameraText1();
	void TutorialCameraZoomEnemyBorn();
	void TutorialCameraEnemyBorn();
	void TutorialCameraReturnPos();
	void TutorialCameraText2Action();

	void TutorialActionClearAfterUpdate(TutorialPhase nextTutorialPhase, SentenceData::SentenceName nextTutorialSentenceName);

private: //定数
	//敵生成座標
	static const XMFLOAT3 enemyBornPos;

private:
	//カメラ
	bool isNormalCamera = true;
	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<TutorialCamera> camera;
	std::unique_ptr<LightCamera> lightCamera;

	//地形
	std::unique_ptr<Field> field;
	//プレイヤー
	std::unique_ptr<Player> player;
	//チュートリアル用敵
	std::unique_ptr<TutorialEnemy> tutorialEnemy;
	//タイマー
	int enemyBornTimer = 0;

	//チュートリアルフェーズ
	TutorialPhase tutorialPhase = TutorialPhase::TUTORIAL_START;
	//チュートリアルカメラフェーズ
	TutorialCameraPhasePhase tutorialCameraPhasePhase = TutorialCameraPhasePhase::TUTORIAL_CAMERA_TEXT_1;
	//各フェーズの更新処理
	std::vector<std::function<void()>> tutorialFunc;
	//カメラ説明フェーズ内フェーズの更新処理
	std::vector<std::function<void()>> tutorialCameraFunc;
	//OK表示スプライト
	std::unique_ptr<OKSprite> okSprite;
	//チュートリアルお試し行動がクリアしているか
	bool isTutorialActionClear = false;
	//チュートリアルお試し行動がクリア後の待機時間タイマー
	std::unique_ptr<Engine::Timer> tutorialActionClearTimer;

	//ロックオン
	std::unique_ptr<LockonUI> lockonUI;

	//行動入力設定
	std::unique_ptr<ActionInputConfig> actionInputConfig;
	bool isInputConfigMode = false;

	//シーン遷移待機中か
	bool isSceneChangeWait = false;
};

