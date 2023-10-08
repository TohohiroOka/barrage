#pragma once
#include "../game/enemy/BaseAction.h"
#include "Object/3d/Object3d.h"
#include <array>
#include <functional>

class Boss1NearAttack1 : public BaseAction
{
private:

	enum class State {
		start,
		beforeMove,
		attack,
		afterMove,
		end,
	};

	struct ObjectInfo {
		std::unique_ptr<Object3d> object;
		float timer;
		int hokanPointNum;//補間番号
		DirectX::XMFLOAT3 pos;
	};

public:
	Boss1NearAttack1();
	~Boss1NearAttack1() {};

	void Update() override;

	void Draw() override;

	bool End() override;

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

