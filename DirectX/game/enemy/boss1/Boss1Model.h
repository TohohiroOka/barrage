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
		attack1 = 0,
		back_end = 1,
		back_start = 2,
		front_end = 3,
		front_start = 4,
		standBy = 5,
		walk = 6,
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