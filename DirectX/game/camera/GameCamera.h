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

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ロックオン
	/// </summary>
	/// <param name="lockonTarget">ロックオンターゲットオブジェクト</param>
	void Lockon(Base3D* lockonTarget);

	//getter
	const XMFLOAT3& GetCameraRota() { return rotation; }
	Base3D* GetLockonTarget() { return lockonTarget; }
	bool GetisLockon() { return isLockon; }
	bool GetisLockonStart() { return isLockonStart; }

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

	/// <summary>
	/// トランスフォーム更新
	/// </summary>
	void UpdateTransform();

	/// <summary>
	/// 回転を更新
	/// </summary>
	void Rotate();

	/// <summary>
	/// 座標を更新
	/// </summary>
	void UpdatePosition();

	/// <summary>
	/// ロックオン入力の挙動
	/// </summary>
	void LockonInput();

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

	//ロックオンターゲット
	Base3D* lockonTarget = nullptr;
	//ロックオン状態か
	bool isLockon = false;
	//ロックオンターゲットを検出するか
	bool isLockonStart = false;
};