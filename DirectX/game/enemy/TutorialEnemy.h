#pragma once
#include "Object/3d/Object3d.h"

/// <summary>
/// チュートリアル用敵
/// </summary>
class TutorialEnemy
{
public: //メンバ関数
	TutorialEnemy(const DirectX::XMFLOAT3& position);
	~TutorialEnemy();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ダメージを食らう
	/// </summary>
	void Damage();

	//getter
	Object3d* GetObject3d() { return object.get(); }
	bool GetIsDamage() { return isDamage; }

private: //メンバ変数
	//オブジェクト3d
	std::unique_ptr<Object3d> object;
	std::unique_ptr<Model> model;

	//ダメージフラグ
	bool isDamage = false;
};