#pragma once
#include <array>

namespace Sound {
	enum class SoundName {
		msp_bgm,
		attack,
		attack_hit,
		jump,
		avoid,
		damage,
		size,
	};

	static std::array<std::string, int(SoundName::size)> SoundNameStr = {
		"msp_bgm",
		"attack",
		"attack_hit",
		"jump",
		"avoid",
		"damage",
	};
}