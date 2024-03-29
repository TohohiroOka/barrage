#pragma once
#include "Object/3d/Object3d.h"
#include "Math/Vector3.h"
#include "player/PlayerData.h"
#include "../effect/PortalEffect.h"

class InterfaceScene;

/// <summary>
/// シーン同士を繋ぐポータル
/// </summary>
class Portal
{
public: //メンバ関数
	Portal(const Vector3& position, InterfaceScene* changeScene);
	~Portal();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="playerRota">プレイヤーの回転角</param>
	/// <param name="isPlayerOnGround">プレイヤーが地面に接地しているか</param>
	void Update(const PlayerData& playerData);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 影用光源カメラ視点描画
	/// </summary>
	void DrawLightView();

	//getter
	Object3d* GetObject3d() { return object.get(); }
	const Vector3& GetIntoPortalRangeMin() { return intoPortalRangeMin; }
	const Vector3& GetIntoPortalRangeMax() { return intoPortalRangeMax; }
	bool GetIsIntoPortal() { return isIntoPortal; }
	InterfaceScene* GetChangeScene() { return changeScene; }

private: //メンバ関数
	/// <summary>
	/// ポータルに入れる範囲にいるかチェック
	/// </summary>
	/// <param name="playerData">プレイヤーの情報</param>
	/// <returns>ポータルに入れる範囲にいるか</returns>
	bool CheckIntoPortalArea(const PlayerData& playerData);

	/// <summary>
	/// プレイヤーの視線内にポータルがあるかチェック
	/// </summary>
	/// <param name="playerData">プレイヤーの情報</param>
	/// <returns>プレイヤーの視線内にポータルがあるか</returns>
	bool CheckLineSightInPortal(const PlayerData& playerData);

private: //メンバ変数
	//オブジェクト3d
	std::unique_ptr<Object3d> object;
	std::unique_ptr<Model> model;
	//ポータルに入れる範囲最小値と最大値
	Vector3 intoPortalRangeMin;
	Vector3 intoPortalRangeMax;
	//ポータルに入れるか
	bool isIntoPortal = false;
	//変更後のシーン
	InterfaceScene* changeScene;

	std::unique_ptr<PortalEffect> effect;
};