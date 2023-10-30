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

	Boss1Model();
	~Boss1Model() {}

	void Update() override;

	void Draw() override;

private:

	std::array<std::unique_ptr<Model>, int(ObjectType::size)> objModel;

};