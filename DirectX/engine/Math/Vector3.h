#pragma once
#include <DirectXMath.h>

class Vector3 : public DirectX::XMFLOAT3
{
public:
	Vector3();
	Vector3(float x, float y,float z);
	Vector3(DirectX::XMFLOAT3 a);

	float length() const;
	Vector3& normalize();
	float dot(const Vector3& v) const;
	Vector3 cross(const Vector3& v) const;
	float angle(const Vector3& v) const;

	/// <summary>
	/// 方向の角度取得
	/// </summary>
	/// <param name="vec">速度ベクトル</param>
	/// <param name="isMinusYRotaFix">y軸角度が負の数の場合正の数に修正するか</param>
	/// <returns>方向の角度</returns>
	static const Vector3 VelocityRotate(const Vector3& vec, const bool isMinusYRotaFix = false);

	//XMVECTOR系の変換関数
	Vector3 DirectXVector3Transform(const DirectX::XMMATRIX& v) const;

	//単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	//代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};

//二項演算子オーバーロード
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);
const bool operator==(const Vector3& a, const Vector3& b);
const bool operator!=(const Vector3& a, const Vector3& b);
const bool operator<(const Vector3& a, const Vector3& b);
const bool operator>(const Vector3& a, const Vector3& b);
const bool operator<=(const Vector3& a, const Vector3& b);
const bool operator>=(const Vector3& a, const Vector3& b);
const Vector3 MatrixTransformPosition(const Vector3& v, const DirectX::XMMATRIX& m);
const Vector3 MatrixTransformDirection(const Vector3& v, const DirectX::XMMATRIX& m);
const Vector3 MatrixTransformWDivision(const Vector3& v, const DirectX::XMMATRIX& m);

//線形補間
const Vector3 lerp(const Vector3& start, const Vector3& end, const float time);
const Vector3 easeIn(const Vector3& start, const Vector3& end, const float time);
const Vector3 easeOut(const Vector3& start, const Vector3& end, const float time);
const Vector3 easeInOut(const Vector3& start, const Vector3& end, const float time);
