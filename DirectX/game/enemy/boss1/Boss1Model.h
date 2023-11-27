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
		back_end = 2,
		back_start = 3,
		fear = 4,
		front_end = 5,
		front_start = 6,
		nearAttak_end = 7,
		nearAttak_start = 8,
		standBy = 9,
		walk = 10,
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