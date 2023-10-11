#pragma once
#include "../game/enemy/BaseAction.h"
#include "Object/3d/Object3d.h"
#include <array>
#include <functional>

/// <summary>
/// 薙ぎ払い
/// </summary>
class Boss1NearAttack1 : public BaseAction
{
private:

	enum class State {
		start,
		beforeMove,
		attack,
		afterMove,
		end,
		non,
	};

	struct ObjectInfo {
		std::unique_ptr<Object3d> object;
		float timer;
		DirectX::XMFLOAT3 pos;
		int hokanPointNum;
		int stateInState;//行動内での処理番号
	};

public:
	Boss1NearAttack1();
	~Boss1NearAttack1() {};

	void Update() override;

	void Draw() override;

private:

	void StartMove();

	void BeforeMove();

	void Attack();

	void afterMove();

	void EndMove();

private:
	static const int objectNum = 10;
	std::unique_ptr<Model> model;
	std::array<ObjectInfo, objectNum> object;
	//現在の動き
	std::vector<std::function<void()>> func_;
	//状態
	State state;
	//全体タイマー
	float timer;
	//回転角度
	float allRota;
};

