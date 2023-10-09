#pragma once
#include "Camera/Camera.h"

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
	/// <param name="_mode">true->3D/false->2D</param>
	static std::unique_ptr<DebugCamera> Create(const XMFLOAT3& targetPos);

public: //メンバ関数
	DebugCamera(const XMFLOAT3& targetPos);
	~DebugCamera() override;

	void Update() override;

private: //メンバ関数
	void MoveCamera();
	void ChangeTargetDistance();
	void UpdateAngle3d(const XMFLOAT2& angle);

private: //静的メンバ変数
	static const float eyeTargetDistance;

private: //メンバ変数
	// カメラ注視点までの距離
	float distance = 200;
	//画面サイズのスケール
	XMFLOAT2 scale;
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
};