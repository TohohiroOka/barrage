#pragma once
#include "../BaseBossModel.h"
#include "effect/SlashEffect.h"

/// <summary>
/// 直線移動（イージングあり）
/// </summary>
class Boss1Model : public BaseBossModel
{
private:

	enum class ObjectType {
		core,
		sword,
		size,
	};
	
public:

	enum class Movement {
		attack1_end = 0,
		attack1_start = 1,
		attack2_end = 2,
		attack2_start = 3,
		back_end = 4,
		back_start = 5,
		crossCut_end=6,
		crossCut_start=7,
		fear = 8,
		front_end = 9,
		front_start = 10,
		nearAttak_end = 11,
		nearAttak_start = 12,
		runAttack_end = 13,
		runAttack_start = 14,
		standBy = 15,
	};

	enum class AttachName {
		core1,
		core2,
		LowerLeg_L,
		LowerLeg_R,
		LowerArm_L,
		LowerArm_R,
		LowerArm_R_Non,
	};

	enum class EffectName {
		sword,
		size,
	};

public:

	Boss1Model();
	~Boss1Model() {}

	void Update() override;

	void Draw() override;

	void DrawLightView() override;

	void ChangesPos(const int _num, const float _maxTime, const Vector3& _pos) override;

	void ChangesScale(const int _num, const float _maxTime, const Vector3& _scale) override;

	void ModelReset() override;

	void IsAttachEffect(int _num, bool _isAlive) override {
		effectAlive[_num] = _isAlive;
	}

	void ChangeInfo();

private:

	std::array<std::unique_ptr<Model>, int(ObjectType::size)> objModel;
	std::unique_ptr<SlashEffect> swordEffect;
	std::array<bool, int(EffectName::size)>effectAlive;
};