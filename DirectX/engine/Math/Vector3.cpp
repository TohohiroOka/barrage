#include "Vector3.h"
#include <cmath>

//コンストラクタ
Vector3::Vector3()
	:Vector3(0,0,0)
{
}

// 値を用いての初期化
Vector3::Vector3(float x, float y, float z)
	:DirectX::XMFLOAT3(x, y, z)
{
}

Vector3::Vector3(DirectX::XMFLOAT3 a)
	:Vector3(a.x, a.y, a.z)
{
}

// ベクトルの大きさ
float Vector3::length() const {
	return float(sqrt(x * x + y * y + z * z));
}

// ベクトルの正規化
Vector3& Vector3::normalize() {
	float len = length();
	if (len != 0)
	{
		*this /= len;
	}
	return *this;
}

// ベクトルの内積
float Vector3::dot(const Vector3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

//ベクトルの外積
Vector3 Vector3::cross(const Vector3& v) const {
	Vector3 temp;
	temp.x = this->y * v.z - this->z * v.y;
	temp.y = this->z * v.x - this->x * v.z;
	temp.z = this->x * v.y - this->y * v.x;
	return temp;
}

float Vector3::angle(const Vector3& v) const
{
	//　※ベクトルの長さが0だと答えが出ませんので注意してください。
	
	//ベクトルAとBの長さを計算する
	float length_A = powf((this->x * this->x) + (this->y * this->y) + (this->z * this->z), 0.5);
	float length_B = powf((v.x * v.x) + (v.y * v.y) + (v.z * v.z), 0.5);

	//内積とベクトル長さを使ってcosθを求める
	float cos_sita = this->dot(v) / (length_A * length_B);

	//cosθからθを求める
	float radian = acos(cos_sita);

	return DirectX::XMConvertToDegrees(radian);
}

Vector3 Vector3::DirectXVector3Transform(const DirectX::XMMATRIX& v) const
{
	DirectX::XMVECTOR XMv = { this->x,this->y,this->z };
	DirectX::XMVECTOR transv = DirectX::XMVector3Transform(XMv, v);

	return Vector3(transv.m128_f32[0], transv.m128_f32[1], transv.m128_f32[2]);
}

//単項演算子+の追加
Vector3 Vector3::operator+() const {
	return *this;
}

//単項演算子+の追加
Vector3 Vector3::operator-() const {
	return Vector3(-x, -y, -z);
}

//+=の追加
Vector3& Vector3:: operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

//-=の追加
Vector3& Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

//*=の追加
Vector3& Vector3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

// /=の追加
Vector3& Vector3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

// ベクトルの加算
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

// ベクトルの減算
const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}

//ベクトルの乗算
const Vector3 operator*(float s, const Vector3& v) {
	return v * s;
}

// ベクトルの除算
const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp /= s;
}

const bool operator==(const Vector3& a, const Vector3& b) {
	return (a.x == b.x) * (a.y == b.y) * (a.z == b.z);
}

const bool operator!=(const Vector3& a, const Vector3& b) {
	return (a.x != b.x) || (a.y != b.y) || (a.z != b.z);
}

const bool operator<(const Vector3& a, const Vector3& b) {
	return (a.x < b.x) && (a.y < b.y) && (a.z < b.z);
}

const bool operator>(const Vector3& a, const Vector3& b) {
	return (a.x > b.x) && (a.y >= b.y) && (a.z > b.z);
}

const bool operator<=(const Vector3& a, const Vector3& b) {
	return (a.x <= b.x) && (a.y <= b.y) && (a.z <= b.z);
}

const bool operator>=(const Vector3& a, const Vector3& b) {
	return (a.x >= b.x) && (a.y >= b.y) && (a.z >= b.z);
}

const Vector3 MatrixTransformPosition(const Vector3& v, const DirectX::XMMATRIX& m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	Vector3 result{
		(v.x * m.r[0].m128_f32[0] + v.y * m.r[1].m128_f32[0] + v.z * m.r[2].m128_f32[0] + m.r[3].m128_f32[0]) / w,
		(v.x * m.r[0].m128_f32[1] + v.y * m.r[1].m128_f32[1] + v.z * m.r[2].m128_f32[1] + m.r[3].m128_f32[1]) / w,
		(v.x * m.r[0].m128_f32[2] + v.y * m.r[1].m128_f32[2] + v.z * m.r[2].m128_f32[2] + m.r[3].m128_f32[2]) / w,
	};

	return result;
}

const Vector3 MatrixTransformDirection(const Vector3& v, const DirectX::XMMATRIX& m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	Vector3 result{
		(v.x * m.r[0].m128_f32[0] + v.y * m.r[1].m128_f32[0] + v.z * m.r[2].m128_f32[0]) / w,
		(v.x * m.r[0].m128_f32[1] + v.y * m.r[1].m128_f32[1] + v.z * m.r[2].m128_f32[1]) / w,
		(v.x * m.r[0].m128_f32[2] + v.y * m.r[1].m128_f32[2] + v.z * m.r[2].m128_f32[2]) / w,
	};

	return result;
}

const Vector3 MatrixTransformWDivision(const Vector3& v, const DirectX::XMMATRIX& m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	Vector3 result{
		(v.x * m.r[0].m128_f32[0] + v.y * m.r[1].m128_f32[0] + v.z * m.r[2].m128_f32[0] + m.r[3].m128_f32[0]) / w,
		(v.x * m.r[0].m128_f32[1] + v.y * m.r[1].m128_f32[1] + v.z * m.r[2].m128_f32[1] + m.r[3].m128_f32[1]) / w,
		(v.x * m.r[0].m128_f32[2] + v.y * m.r[1].m128_f32[2] + v.z * m.r[2].m128_f32[2] + m.r[3].m128_f32[2]) / w,
	};

	result /= result.z;

	return result;
}


//線形補間
const Vector3 lerp(const Vector3& start, const Vector3& end, const float time) {
	return start * (1.0f - time) + end * time;
}

const Vector3 easeIn(const Vector3& start, const Vector3& end, const float time) {
	float t = time * time;
	return start * (1.0f - t) + end * t;
}

const Vector3 easeOut(const Vector3& start, const Vector3& end, const float time) {
	float t = time * (2 - time);
	return start * (1.0f - t) + end * t;
}

const Vector3 easeInOut(const Vector3& start, const Vector3& end, const float time) {
	float t = time * time * (3 - (2 * time));
	return start * (1.0f - t) + end * t;
}
