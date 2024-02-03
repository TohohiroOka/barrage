#pragma once
#include "../game/enemy/BaseAction.h"
#include <DirectXMath.h>
#include "../Math/Timer.h"
#include <functional>

/// <summary>
/// 壁際の時に中央寄りに移動する
/// </summary>
class Boss1Move2 : public BaseAction
{
private:

	enum class State {
		up,
		side,
		non,
	};

public:
	Boss1Move2();
	~Boss1Move2() {};

	void Update() override;

	void Draw() override {};

	void FrameReset() override {};

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override {};

	void GetDamageInfo(int& _damageNum, int& _knockbackPower, int& _knockbackTime, bool& _isKnockbackStart) override {
		_damageNum = 0;
		_knockbackPower = 0;
		_knockbackTime = 0;
		_isKnockbackStart = false;
	}

	bool SetCameraTarget() override { return true; }

	void UpMove();

	void SideMove();

private:

	//開始地点
	DirectX::XMFLOAT3 startPos;
	//移動後地点
	DirectX::XMFLOAT3 endPos;
	//イージングタイマー
	std::unique_ptr<Engine::Timer> timer;
	//現在の動き
	State state;
	std::vector<std::function<void()>> func_;

};