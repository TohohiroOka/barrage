#pragma once
#include <DirectXMath.h>

//二項演算子オーバーロード XMFLOAT2
const DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2);
const DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2);
const DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& v, float s);
const DirectX::XMFLOAT2 operator*(float s, const DirectX::XMFLOAT2& v);
const DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& v, float s);
const bool operator==(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
const bool operator!=(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
const bool operator<(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
const bool operator>(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
const bool operator<=(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
const bool operator>=(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);

//二項演算子オーバーロード XMFLOAT3
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& v, float s);
const DirectX::XMFLOAT3 operator*(float s, const DirectX::XMFLOAT3& v);
const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& v, float s);
const bool operator==(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
const bool operator!=(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
const bool operator<(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
const bool operator>(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
const bool operator<=(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
const bool operator>=(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
