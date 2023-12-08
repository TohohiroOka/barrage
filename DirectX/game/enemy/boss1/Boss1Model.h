#pragma once
#include "../BaseBossModel.h"
#include "../Math/Timer.h"

/// <summary>
/// 直線移動（イージングあり）
/// </summary>
class Boss1Model : public BaseBossModel
{
private:

	enum class ObjectType {
		cube,
		cone,
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

public:

	Boss1Model();
	~Boss1Model() {}

	void Update() override;

	void Draw() override;

	void DrawLightView() override;

private:

	std::array<std::unique_ptr<Model>, int(ObjectType::size)> objModel;

};