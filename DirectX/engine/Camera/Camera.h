#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
#include "Math/Vector3.h"

/// <summary>
/// カメラ基本機能
/// </summary>
class Camera
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://静的メンバ関数

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns></returns>
	/// <param name="_mode">true->3D/false->2D</param>
	static std::unique_ptr<Camera> Create(const bool _mode = true);

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_mode">true->3D/false->2D</param>
	Camera(const bool _mode);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Camera() = default;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// シェイク開始
	/// </summary>
	/// <param name="shakePower">シェイクの強さ</param>
	/// <param name="shakeTime">シェイクする時間</param>
	void ShakeStart(const float _shakePower = 20, const float _shakeTime = 30);

	/// <summary>
	/// シェイク
	/// </summary>
	void Shake();

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	inline const XMMATRIX& GetView() { return matView; }

	/// <summary>
	/// 射影行列の取得
	/// </summary>
	/// <returns>射影行列</returns>
	inline const XMMATRIX& GetProjection() { return matProjection; }

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	inline const Vector3& GetEye() { return eye; }

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	inline const Vector3& GetTarget() { return target; }

	/// <summary>
	/// カメラの傾きの取得
	/// </summary>
	/// <returns>上方向ベクトル</returns>
	inline const Vector3& GetUp() { return up; }

	/// <summary>
	/// 視点座標セット
	/// </summary>
	/// <param name="eye">視点座標</param>
	void SetEye(const Vector3& _eye) { this->eye = _eye; }

	/// <summary>
	/// 注視点座標セット
	/// </summary>
	/// <param name="position">注視点座標</param>
	void SetTarget(const Vector3& _target) { this->target = _target; }

	/// <summary>
	/// カメラの傾きのセット
	/// </summary>
	/// <param name="_up">カメラの傾き</param>
	void SetUp(const Vector3& _up) { up = _up; }

	/// <summary>
	/// 射影行列の再設定
	/// </summary>
	/// <param name="_back">最奥</param>
	void SetMatProjection(float _back);

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move"></param>
	void MoveVector(const XMVECTOR& move);

protected: // メンバ変数
	// ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// 射影行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// 視点座標
	Vector3 eye = { 0, 0, -20 };
	// 注視点座標
	Vector3 target = { 0, 0, 0 };
	// 上方向ベクトル
	Vector3 up = { 0, 1, 0 };
	// アスペクト比
	float aspectRatio = 1.0f;
	//シェイクするか
	bool isShake = false;
	//カメラシェイク用タイマー
	int32_t shakeTimer = 0;
	//シェイクする時間
	float shakeTime = 0;
	//シェイク最大の強さ
	float maxShakePower = 0;
};