#pragma once
#include "Singleton.h"
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <memory>
#include <random>
#include <cstdlib>
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include "../Math/DirectXMathHelper.h"

class GameHelper : public Singleton<GameHelper>
{
	friend class Singleton<GameHelper>;

public:
	GameHelper(){};
	~GameHelper(){};

	void Update();

	/// <summary>
	/// 桁数取得
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	unsigned GetDigit(unsigned num);

	/// <summary>
	/// 二点間の角度取得
	/// </summary>
	/// <param name="_startPoint">座標1</param>
	/// <param name="_endPoint">座標2</param>
	/// <returns></returns>
	float GetAngle(const DirectX::XMFLOAT2& _startPoint, const DirectX::XMFLOAT2& _endPoint);

	/// <summary>
	/// 乱数生成
	/// 0から範囲までの乱数を出力
	/// </summary>
	/// <param name="range">範囲</param>
	/// <returns>乱数</returns>
	int RandomInt(int _range);

	/// <summary>
	/// 乱数生成
	/// 1か-1を出力
	/// </summary>
	/// <returns>1or-1</returns>
	int RandomSign();

	/// <summary>
	/// 円状に乱数を生成。均一化有り
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 GetRandInUniformInCircle();

	/// <summary>
	/// 円状に乱数を生成。均一化有り
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 GetHalfRandInUniformInCircle();

	/// <summary>
	/// valueのm桁目からn桁目を取得する
	/// </summary>
	/// <param name="value">取得先となる浮動小数点数</param>
	/// <param name="m">取得を開始する桁</param>
	/// <param name="n">取得を終了する桁</param>
	/// <returns></returns>
	int GetDigits(int value, int m, int n);

	/// <summary>
	/// 線形補間float
	/// </summary>
	/// <param name="points">制御点の集合</param>
	/// <param name="startIndex">保管する区間の添字</param>
	/// <param name="t">時間経過</param>
	/// <returns></returns>
	float SplinePosition(const std::vector<float>& points, int startIndex, float t);

	/// <summary>
	/// 線形補間Vector2
	/// </summary>
	/// <param name="points">制御点の集合</param>
	/// <param name="startIndex">保管する区間の添字</param>
	/// <param name="t">時間経過</param>
	/// <returns></returns>
	Vector2 SplinePosition(const std::vector<Vector2>& points, int startIndex, float t);

	/// <summary>
	/// 線形補間Vector3
	/// </summary>
	/// <param name="points">制御点の集合</param>
	/// <param name="startIndex">保管する区間の添字</param>
	/// <param name="t">時間経過</param>
	/// <returns></returns>
	Vector3 SplinePosition(const std::vector<Vector3>& points, int startIndex, float t);

	/// <summary>
	/// 方向の角度取得
	/// </summary>
	/// <param name="vec">速度ベクトル</param>
	/// <param name="isMinusYRotaFix">y軸角度が負の数の場合正の数に修正するか</param>
	/// <returns>方向の角度</returns>
	static const Vector3 VelocityRotate(const Vector3& vec, const bool isMinusYRotaFix = false);

	//ゲーム速度を変更する
	void SetGameSpeed(float _gameSpeed) { gameSpeed = _gameSpeed; }

	/// <summary>
	/// スロウ
	/// </summary>
	/// <param name="_gameSpeed">ゲーム速度</param>
	/// <param name="_timer">処理時間</param>
	void SetSlow(float _gameSpeed, float _timer) {
		gameSpeed = _gameSpeed;
		slowTimer = _timer;
	}

	const float mapSize = 255.0f * 10;
	//ゲーム速度
	float gameSpeed = 1;
	//スロウ処理を何フレーム行うか
	float slowTimer;
};