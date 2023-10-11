#include "../Math/DirectXMathHelper.h"

using namespace DirectX;

const XMFLOAT2 operator+(const XMFLOAT2& v1, const XMFLOAT2& v2) {
	XMFLOAT2 result = v1;
	result.x += v2.x;
	result.y += v2.y;
	return result;
}

const XMFLOAT2 operator-(const XMFLOAT2& v1, const XMFLOAT2& v2) {
	XMFLOAT2 result = v1;
	result.x -= v2.x;
	result.y -= v2.y;
	return result;
}

const XMFLOAT2 operator*(const XMFLOAT2& v, float s) {
	XMFLOAT2 result = v;
	result.x *= s;
	result.y *= s;
	return result;
}

const XMFLOAT2 operator*(float s, const XMFLOAT2& v) {
	XMFLOAT2 result = v;
	result.x *= s;
	result.y *= s;
	return result;
}

const XMFLOAT2 operator/(const XMFLOAT2& v, float s) {
	XMFLOAT2 result = v;
	result.x /= s;
	result.y /= s;
	return result;
}

const bool operator==(const XMFLOAT2& a, const XMFLOAT2& b) {
	return (a.x == b.x) * (a.y == b.y);
}

const bool operator!=(const XMFLOAT2& a, const XMFLOAT2& b) {
	return (a.x != b.x) || (a.y != b.y);
}

const bool operator<(const XMFLOAT2& a, const XMFLOAT2& b) {
	return (a.x < b.x) && (a.y < b.y);
}

const bool operator>(const XMFLOAT2& a, const XMFLOAT2& b) {
	return (a.x > b.x) && (a.y >= b.y);
}

const bool operator<=(const XMFLOAT2& a, const XMFLOAT2& b) {
	return (a.x <= b.x) && (a.y <= b.y);
}

const bool operator>=(const XMFLOAT2& a, const XMFLOAT2& b) {
	return (a.x >= b.x) && (a.y >= b.y);
}


const XMFLOAT3 operator+(const XMFLOAT3& v1, const XMFLOAT3& v2) {
	XMFLOAT3 result = v1;
	result.x += v2.x;
	result.y += v2.y;
	result.z += v2.z;
	return result;
}

// ベクトルの減算
const XMFLOAT3 operator-(const XMFLOAT3& v1, const XMFLOAT3& v2) {
	XMFLOAT3 result = v1;
	result.x -= v2.x;
	result.y -= v2.y;
	result.z -= v2.z;
	return result;
}

const XMFLOAT3 operator*(const XMFLOAT3& v, float s) {
	XMFLOAT3 result = v;
	result.x *= s;
	result.y *= s;
	result.z *= s;
	return result;
}

//ベクトルの乗算
const XMFLOAT3 operator*(float s, const XMFLOAT3& v) {
	XMFLOAT3 result = v;
	result.x *= s;
	result.y *= s;
	result.z *= s;
	return result;
}

// ベクトルの除算
const XMFLOAT3 operator/(const XMFLOAT3& v, float s) {
	XMFLOAT3 result = v;
	result.x /= s;
	result.y /= s;
	result.z /= s;
	return result;
}

const bool operator==(const XMFLOAT3& a, const XMFLOAT3& b) {
	return (a.x == b.x) * (a.y == b.y) * (a.z == b.z);
}

const bool operator!=(const XMFLOAT3& a, const XMFLOAT3& b) {
	return (a.x != b.x) || (a.y != b.y) || (a.z != b.z);
}

const bool operator<(const XMFLOAT3& a, const XMFLOAT3& b) {
	return (a.x < b.x) && (a.y < b.y) && (a.z < b.z);
}

const bool operator>(const XMFLOAT3& a, const XMFLOAT3& b) {
	return (a.x > b.x) && (a.y >= b.y) && (a.z > b.z);
}

const bool operator<=(const XMFLOAT3& a, const XMFLOAT3& b) {
	return (a.x <= b.x) && (a.y <= b.y) && (a.z <= b.z);
}

const bool operator>=(const XMFLOAT3& a, const XMFLOAT3& b) {
	return (a.x >= b.x) && (a.y >= b.y) && (a.z >= b.z);
}
