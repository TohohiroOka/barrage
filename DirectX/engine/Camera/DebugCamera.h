#pragma once
#include "Camera/Camera.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"

/// <summary>
/// デバッグカメラ
/// </summary>
class DebugCamera : public Camera
{
public: //静的メンバ関数
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<DebugCamera> Create(const Vector3& targetPos);

public: //メンバ関数
	DebugCamera(const Vector3& targetPos);
	~DebugCamera() override;

	void Update() override;

private: //メンバ関数
	void MoveCamera();
	void ChangeTargetDistance();
	void UpdateAngle3d(const XMFLOAT2& angle);

	void Move1();

private: //静的メンバ変数
	static const float eyeTargetDistance;

private: //メンバ変数
	// カメラ注視点までの距離
	float distance = 200;
	//画面サイズのスケール
	XMFLOAT2 scale;
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	//動き用
	Vector2 oka_moveRota;
	Vector3 pos;

};