#pragma once
#include "Camera/GameCamera.h"
#include <functional>

/// <summary>
/// タイトルシーン用カメラ
/// </summary>
class TitleCamera :public GameCamera
{
public: //enum
	/// <summary>
	/// タイトルカメラの状態
	/// </summary>
	enum class TitleCameraPhase
	{
		NORMAL, //通常状態
		MOVE_PORTAL_FRONT, //ポータル正面に移動
		ZOOM_PORTAL, //ポータルをズーム
		STAY,	//停止
	};


public: //メンバ関数
	TitleCamera();
	~TitleCamera() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 状態の変更
	/// </summary>
	/// <param name="phase">変更後のフェーズ</param>
	void ChangePhase(TitleCameraPhase phase);

	//getter
	TitleCameraPhase GetTitleCameraPhase() { return cameraPhase; }
	bool GetIsPhaseActionEnd() { return isPhaseActionEnd; }

	//setter
	void SetPortalPos(const Vector3& portalPos) { this->portalPos = portalPos; }

private: //メンバ関数
	/// <summary>
	/// 通常状態の行動
	/// </summary>
	void NormalAction();

	/// <summary>
	/// ポータルの正面に移動する行動
	/// </summary>
	void MovePortalFrontAction();

	/// <summary>
	/// ポータルをズームする行動
	/// </summary>
	void ZoomPortalAction();

	/// <summary>
	/// ポータル前で停止する行動
	/// </summary>
	void StayAction();

private: //メンバ変数
	//カメラのフェーズ状態
	TitleCameraPhase cameraPhase = TitleCameraPhase::NORMAL;
	//各行動の動き
	std::vector<std::function<void()>> func;
	//対象ポータルの座標
	Vector3 portalPos;
	//イージング用タイマー
	std::unique_ptr<Engine::Timer> easeTimer;
	//イージング前座標
	Vector3 easeBeforePos;
	//イージング後座標
	Vector3 easeAfterPos;
	//イージング後回転角
	Vector3 easeAfterRota;
	//状態の行動を終えたか(通常行動に終わりはない)
	bool isPhaseActionEnd = false;
};