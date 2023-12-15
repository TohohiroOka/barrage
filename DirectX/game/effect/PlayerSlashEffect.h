#pragma once
#include "Particle/Emitter.h"
#include "Object/3d/Object3d.h"
#include "Math/Vector3.h"

/// <summary>
/// プレイヤー斬撃演出
/// </summary>
class PlayerSlashEffect
{
public: //メンバ関数
	PlayerSlashEffect(const std::string& texName);
	~PlayerSlashEffect(){};

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="swordHandlePos">剣の持ち手座標</param>
	/// <param name="swordTipPos">剣先座標</param>
	void Update(const Vector3& swordHandlePos, const Vector3& swordTipPos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: //メンバ関数
	/// <summary>
	/// 発生座標更新
	/// </summary>
	/// <param name="swordHandlePos">剣の持ち手座標</param>
	/// <param name="swordTipPos">剣先座標</param>
	void UpdateEmitPos(const Vector3& swordHandlePos, const Vector3& swordTipPos);

	/// <summary>
	/// パーティクル発生
	/// </summary>
	void AddParticle();

private: //静的メンバ変数 
	//剣から発生させるパーティクル数
	static const int EmitNum = 10;

private: //メンバ変数
	//パーティクル発生エミッター
	std::unique_ptr<Emitter> particle;
	//発生座標保存用list
	std::array<std::list<Vector3>, EmitNum> emitPosLists;
};

