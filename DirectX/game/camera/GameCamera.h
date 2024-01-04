#pragma once
#include "Camera/Camera.h"
#include "player/Player.h"

/// <summary>
/// ゲームカメラ
/// </summary>
class GameCamera :public Camera
{
public: //静的メンバ関数
	
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
	void UpdateRotate();

	/// <summary>
	/// ロックオン時のターゲットと注視点との距離を更新
	/// </summary>
	void UpdateLockonTargetDistance();

	/// <summary>
	/// ロックオン時の回転を更新
	/// </summary>
	void UpdateLockonRotate();

	/// <summary>
	/// ロックオン時の回転を実行
	/// </summary>
	void LockonAdjastEaseRotate(float& rotation, float easeBeforeRotate, float easeAfterRotate, float easeTime);

	/// <summary>
	/// 座標を更新
	/// </summary>
	void UpdatePosition();

	/// <summary>
	/// 座標更新による地面などのめり込みを解消
	/// </summary>
	void PositionCollision();

	/// <summary>
	/// ロックオン入力の挙動
	/// </summary>
	void LockonInput();

	/// <summary>
	/// ロックオン解除時に回転角を戻す
	/// </summary>
	void LockonEndRotate();

private: //静的メンバ変数
	static Player* player;
	static const float rotateXMin;
	static const float rotateXMax;
	static const float rotateXBase;
	static const float rotateCenterDistanceNormal;
	static const Vector3 targetDistanceNormal;

private: //メンバ変数
	//カメラ座標
	Vector3 position;
	//ターゲット角度
	Vector3 rotation;
	//回転の中心となるターゲットとの距離
	float rotateCenterDistance;
	//ターゲットの座標保存用(数フレーム遅らせてカメラを追従させるため)
	std::list<Vector3> targetPositionsKeep;
	//ターゲットの中心から視点をずらす距離
	Vector3 targetDistance;
	//ワールド変換行列
	XMMATRIX matWorld = {};

	//ロックオンターゲット
	Base3D* lockonTarget = nullptr;
	//注視点とロックオンターゲットの中心との距離
	Vector3 lockonTargetDistance;
	//ロックオン状態か
	bool isLockon = false;
	//ロックオンターゲットを検出するか
	bool isLockonStart = false;
	//ロックオン解除時に回転角を戻すか
	bool isLockonEndRotate = false;
	//ロックオン開始時にロックオン回転角にイージングする時間タイマー
	std::unique_ptr<Engine::Timer> lockonChangeRotaTimer;
	//ロックオン時にプレイヤーとロックオン対象のy軸が重なったときにy軸回転を停止させるか
	bool isLockonRotYStop = false;
	//ロックオン時にプレイヤーとロックオン対象のy軸が重なったときにy軸回転を停止させる時間タイマー
	std::unique_ptr<Engine::Timer> lockonRotYStopTimer;
	//ロックオン時にy軸回転を停止させた後、修正に使用する前フレームのプレイヤーとロックオン対象との角度
	float oldMoveRotaVelocity;
	//イージング用変数
	Vector3 easeBeforeRota;
};