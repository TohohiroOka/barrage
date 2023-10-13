#pragma once
#include "../game/enemy/BaseAction.h"
#include <DirectXMath.h>

/// <summary>
/// 直線移動（イージングあり）
/// </summary>
class Boss1Move1 : public BaseAction
{
public:
	Boss1Move1(DirectX::XMFLOAT3 _pos);
	~Boss1Move1() {};

	void Update() override;

	void Draw() {};

	void GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info) override {};


private:

	//開始地点
	DirectX::XMFLOAT3 startPos;
	//移動後地点
	DirectX::XMFLOAT3 endPos;
	//イージングタイマー
	float timer;
};

