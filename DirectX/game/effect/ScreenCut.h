#pragma once
#include "../Object/2d/ShaoeSprite.h"
#include "../Particle/ParticleManager.h"
#include <array>
#include <functional>
#include <forward_list>

class ScreenCut
{
	enum class State {
		light_half,
		light_other,
		panelBreak,
		non,
	};

public:

	ScreenCut();
	~ScreenCut() {};

	void Initialize();

	void Update();

	void Draw();

	static void SetEffect(const bool _isEffect) { isEffect = _isEffect; }

	static void Reset() { isReset = true; }

private:

	void LightHalf();

	void LightOther();

	void PanelBreak();

	void AddSpriteHalf(const float _rate);

	void AddSpriteOther(const float _rate);

private:

	static const int panelNum = 17;
	std::array<std::unique_ptr<Model>, panelNum> model;
	std::array<std::unique_ptr<ShaoeSprite>, panelNum> panel;

	static const int otherNum = 15;
	static const std::array<DirectX::XMFLOAT2, otherNum> otherMin;
	static const std::array<DirectX::XMFLOAT2, otherNum> otherMax;
	static const std::array<float, panelNum> rota;

	//エフェクトを行うか
	static bool isEffect;

	//初期化を行うか
	static bool isReset;

	State state;
	std::vector<std::function<void()>> func_;

	float timer;

	std::array<std::unique_ptr<ParticleManager>, 3> effect;

	DirectX::XMFLOAT2 effectPos;

	float panelMoveSpeed;

	bool isEnd;
};