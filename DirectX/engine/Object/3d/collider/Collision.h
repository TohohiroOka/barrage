﻿#pragma once
#include "CollisionPrimitive.h"
#include "Math/Vector3.h"

class Camera;

/// <summary>
/// 当たり判定ヘルパークラス
/// </summary>
class Collision
{
public:

	/// <summary>
	/// 円と円の当たり判定
	/// </summary>
	/// <param name="pos1">円1の座標</param>
	/// <param name="radius1">円1の半径</param>
	/// <param name="pos2">円2の座標</param>
	/// <param name="radius2">円2の半径</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckCircle2Circle(const DirectX::XMFLOAT3& pos1,
		float radius1, const DirectX::XMFLOAT3& pos2, float radius2);

	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="_point">点</param>
	/// <param name="_triangle">三角形</param>
	/// <param name="_closest">最近接点（出力用）</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& _point,
		const Triangle& _triangle, DirectX::XMVECTOR* _closest);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="_sphereA">球A</param>
	/// <param name="_sphereB">球B</param>
	/// <param name="_inter">交点（出力用）</param>
	/// <param name="_reject">排斥ベクトル（出力用）</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& _sphereA,
		const Sphere& _sphereB, DirectX::XMVECTOR* _inter = nullptr, DirectX::XMVECTOR* _reject = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="_sphere">球</param>
	/// <param name="_plane">平面</param>
	/// <param name="_inter">交点（平面上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere& _sphere,
		const Plane& _plane, DirectX::XMVECTOR* _inter = nullptr);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="_sphere">球</param>
	/// <param name="_triangle">三角形</param>
	/// <param name="_inter">交点（三角形上の最近接点）</param>
	/// <param name="_reject">排斥ベクトル（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere& _sphere,
		const Triangle& _triangle, DirectX::XMVECTOR* _inter = nullptr, DirectX::XMVECTOR* _reject = nullptr);

	/// <summary>
	/// レイと平面（範囲・表裏有）の当たり判定
	/// </summary>
	/// <param name="_lay">レイ</param>
	/// <param name="_box">ボックス</param>
	/// <param name="_boxMat">ボックスのワールド行列</param>
	/// <param name="t">衝突間隔（出力用）</param>
	/// <param name="_inter">交点（出力用）</param>
	/// <returns></returns>
	static bool CheckRayAabb(const Segment& _lay, const Box& _box,
		const DirectX::XMMATRIX& _boxMat, float* _t, DirectX::XMVECTOR* _inter = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="_lay">レイ</param>
	/// <param name="_plane">平面</param>
	/// <param name="_distance">距離（出力用）</param>
	/// <param name="_inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Segment& _lay,
		const Plane& _plane, float* _distance = nullptr, DirectX::XMVECTOR* _inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="_lay">レイ</param>
	/// <param name="_triangle">三角形</param>
	/// <param name="_distance">距離（出力用）</param>
	/// <param name="_inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Segment& _lay,
		const Triangle& _triangle, float* _distance = nullptr, DirectX::XMVECTOR* _inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="_lay">レイ</param>
	/// <param name="_sphere">球</param>
	/// <param name="_sphere">距離（出力用）</param>
	/// <param name="_inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Segment& _lay,
		const Sphere& _sphere, float* _distance = nullptr, DirectX::XMVECTOR* _inter = nullptr);

	/// <summary>
	/// 球とカプセル
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="capsule">カプセル</param>
	/// <param name="_distance">距離（出力用）</param>
	/// <returns></returns>
	static bool CheckSphereCapsule(const Sphere& sphere, const Capsule& capsule, float* distance);

	/// <summary>
	/// カプセルとカプセル
	/// </summary>
	/// <param name="capsule1">カプセル</param>
	/// <param name="capsule2">カプセル</param>
	/// <returns></returns>
	static bool CheckCapsuleCapsule(const Capsule& capsule1, const Capsule& capsule2);

	/// <summary>
	/// 三角形とカプセルの当たり判定
	/// </summary>
	/// <param name="_triangle">三角形</param>
	/// <param name="_capsule">カプセル</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckTriangleCapsule(const Triangle& _triangle, const Capsule& _capsule);

	/// <summary>
	/// 線分と線分の距離の二乗を求める
	/// </summary>
	/// <param name="p1">線分1start</param>
	/// <param name="q1">線分1end</param>
	/// <param name="p2">線分2start</param>
	/// <param name="q2">線分2end</param>
	/// <returns></returns>
	static float sqDistanceSegmentSegment(const Vector3& p1, const Vector3& q1, const Vector3& p2, const Vector3& q2);

	/// <summary>
	/// 線分と線分の当たり判定
	/// </summary>
	/// <param name="p1">線分1start</param>
	/// <param name="q1">線分1end</param>
	/// <param name="p2">線分2start</param>
	/// <param name="q2">線分2end</param>
	/// <returns></returns>
	static bool CheckSegmentSegment(const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& q1,
		const DirectX::XMVECTOR& p2, const DirectX::XMVECTOR& q2,
		float* pOut_dist = nullptr, DirectX::XMVECTOR* pOut_pos1 = nullptr, DirectX::XMVECTOR* pOut_pos2 = nullptr);

	/// <summary>
	/// スクリーン座標からワールド座標への変換
	/// </summary>
	/// <param name="_worldPos">ワールド座標の出力用</param>
	/// <param name="_camera">カメラクラス</param>
	/// <param name="_pos">スクリーン座標</param>
	/// <param name="fz">奥行（0.0f～1.0f）</param>
	static void CalcScreenToWorld(DirectX::XMFLOAT3* _worldPos, Camera* _camera, const DirectX::XMFLOAT2& _pos, const float fz);


	//値を範囲内に収める[low, high]
	static float clamp(float x, float low, float high)
	{
		x = (x < low) ? low : x;
		x = (x < high) ? high : x;
		return x;
	}
};