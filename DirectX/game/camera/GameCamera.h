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
	/// <param name="_mode">true->3D/false->2D</param>
	static std::unique_ptr<GameCamera> Create(const bool _mode = true);
	
	static void SetPlayer(Player* player) { GameCamera::player = player; }

public: //メンバ関数
	GameCamera(const bool _mode);
	~GameCamera() override;

	void Update() override;
	void Move();

	const XMFLOAT3& GetCameraRota() { return cameraRota; }

private: //静的メンバ変数
	static Player* player;

private: //メンバ変数
	//カメラ座標
	XMFLOAT3 cameraPos;
	//ターゲット角度
	XMFLOAT3 cameraRota;
};