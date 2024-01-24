#pragma once
#include "Camera/GameCamera.h"
#include <functional>

/// <summary>
/// チュートリアルシーン用カメラ
/// </summary>
class TutorialCamera :public GameCamera
{
public: //enum
	/// <summary>
	/// チュートリアルカメラの状態
	/// </summary>
	enum class TutorialCameraPhase
	{
		NORMAL, //通常状態
		ZOOM_ENEMY_BORN, //敵発生をズーム
		ZOOM_END_RETURN, //ズームを解除し、元に戻す
	};


public: //メンバ関数
	TutorialCamera();
	~TutorialCamera() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 状態の変更
	/// </summary>
	/// <param name="phase">変更後のフェーズ</param>
	void ChangePhase(TutorialCameraPhase phase);

	//getter
	TutorialCameraPhase GetTutorialCameraPhase() { return cameraPhase; }
	bool GetIsPhaseActionEnd() { return isPhaseActionEnd; }

	//setter
	void SetZoomTargetPos(const Vector3& zoomTargetPos) { this->zoomTargetPos = zoomTargetPos; }

private: //メンバ関数
	/// <summary>
	/// 通常状態の行動
	/// </summary>
	void NormalAction();

	/// <summary>
	/// 敵発生をズームする行動
	/// </summary>
	void ZoomEnemyBornAction();

	/// <summary>
	/// ズームを解除し、元に戻す行動
	/// </summary>
	void ZoomEndReturnAction();

private: //メンバ変数
	//カメラのフェーズ状態
	TutorialCameraPhase cameraPhase = TutorialCameraPhase::NORMAL;
	//各行動の動き
	std::vector<std::function<void()>> func;
	//ズーム対象の座標
	Vector3 zoomTargetPos;
	//イージング用タイマー
	std::unique_ptr<Engine::Timer> easeTimer;
	//イージング前座標
	Vector3 easeBeforePos;
	//イージング後座標
	Vector3 easeAfterPos;
	//イージング後回転角
	Vector3 easeAfterRota;
	//ズーム前座標
	Vector3 zoomBeforePos;
	//ズーム前回転角
	Vector3 zoomBeforeRota;
	//状態の行動を終えたか(通常行動に終わりはない)
	bool isPhaseActionEnd = false;
};