﻿#pragma once
/// <summary>
/// 当たり判定プリミティブ
/// </summary>

#include <DirectXMath.h>
#include <vector>
#include "Math/Vector3.h"

/// <summary>
/// ボックス
/// </summary>
struct Box
{
	// 1
	DirectX::XMFLOAT3 point1 = {};
	// 2
	DirectX::XMFLOAT3 point2 = {};
};

/// <summary>
/// 球
/// </summary>
struct Sphere
{
	// 中心座標
	DirectX::XMVECTOR center = {};
	// 半径
	float radius = 1.0f;
};

/// <summary>
/// 平面
/// </summary>
struct Plane
{
	// 法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0 };
	// 原点(0,0,0)からの距離
	float distance = 0.0f;
};

/// <summary>
/// 法線付き三角形（時計回りが表面）
/// </summary>
class Triangle
{
public:
	// 頂点座標3つ
	DirectX::XMVECTOR	p0;
	DirectX::XMVECTOR	p1;
	DirectX::XMVECTOR	p2;
	// 法線ベクトル
	DirectX::XMVECTOR	normal;

	//八分木
	std::vector<int> Octree;

	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();
};

/// <summary>
/// レイ（半直線）
/// </summary>
struct Segment
{
	// 始点座標
	DirectX::XMVECTOR start = { 0,0,0,1 };
	//終点座標
	DirectX::XMVECTOR end = { 0,0,0,1 };
	// 方向
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};

/// <summary>
/// カプセル
/// </summary>
struct Capsule
{
	//始点
	Vector3 startPosition = { 0,0,0 };
	//終点
	Vector3 endPosition = { 0,0,0 };
	// 半径
	float radius = 1.0f;
};