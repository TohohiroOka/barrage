#pragma once
#include "Camera/Camera.h"
#include "player/Player.h"

/// <summary>
/// ゲームカメラ
/// </summary>
class GameCamera :public Camera
{
public: //静的メンバ関数
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<GameCamera> Create();
	
	static void SetPlayer(Player* player) { GameCamera::player = player; }

public: //メンバ関数
	GameCamera();
	~GameCamera() override;

	void Update() override;

	//getter
	const XMFLOAT3& GetCameraRota() { return rotation; }

private: //メンバ関数
	/// <summary>
	/// ワールド行列を更新
	/// </summary>
	/// <param name="matTrans">平行移動行列</param>
	void UpdateMatWorld(const XMMATRIX& matTrans);

	/// <summary>
	/// 視点と注視点を更新
	/// </summary>
	void UpdateEyeTarget();


	void UpdateTransform();


	void Rotate();

	void Move();

private: //静的メンバ変数
	static Player* player;
	static const float baseDistance;

private: //メンバ変数
	//カメラ座標
	Vector3 position;
	//ターゲット角度
	Vector3 rotation;
	//ターゲットとの距離
	float targetDistance;
	//ワールド変換行列
	XMMATRIX matWorld = {};
};