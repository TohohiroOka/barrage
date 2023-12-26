#pragma once
#include "../Object/2d/ShaoeSprite.h"
#include <array>
#include <functional>
#include "../Math/Timer.h"

class ScreenCut
{
	enum class State {
		light,
		panelBreak,
	};

public:

	ScreenCut();
	~ScreenCut() {};

	void Initialize();

	void Update();

	void Draw();

	void SetEffect() { isEffect = true; }

private:

	void Light();

	void PanelBreak();

private:

	static const int panelNum = 17;
	std::array<std::unique_ptr<Model>, panelNum> model;
	std::array<std::unique_ptr<ShaoeSprite>, panelNum> panel;

	//エフェクトを行うか
	bool isEffect;

	State state;
	std::vector<std::function<void()>> func_;

	std::unique_ptr<Engine::Timer> timer;
};