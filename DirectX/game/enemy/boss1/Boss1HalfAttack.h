#pragma once
#include "../Object/3d/InstanceObject.h"
#include "../Object/3d/PrimitiveObject3D.h"
#include "../BaseAction.h"
#include <functional>

class Boss1HalfAttack : public BaseAction
{
private:

	struct SwordInfo {
		float alpha;
		Vector3 pos;
		Vector3 rota;
		Vector2 angle;
	};

	struct RingInfo{
		std::array<Vector3, 20> linePos;
		std::array<SwordInfo, 20> swordObject;
		float dist;
		float maxDist;
	};

	enum class State {
		start,
		attack,
		end,
		non,
	};

public:
	Boss1HalfAttack();
	~Boss1HalfAttack() {};

	void Update() override;

	void Draw() override;

	void FrameReset() override {
		instanceObject->FrameReset();
		line->ResetVertex();
	}
	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override {};

	int GetDamage()override { return 0; }

	void Start();

	void Attack();

	void End();

	Vector3 GetMoveVec(SwordInfo& info, float dist);

private:
	
	//状態
	State state;
	std::vector<std::function<void()>> func_;

	std::unique_ptr<PrimitiveObject3D> line;

	//リング
	static const int ringNum = 5;
	std::array<RingInfo, ringNum> ringInfo;

	//オブジェクト
	std::unique_ptr<Model> model;
	std::unique_ptr<InstanceObject> instanceObject;
	
	//全体タイマー
	std::unique_ptr<Engine::Timer> timer;

};

