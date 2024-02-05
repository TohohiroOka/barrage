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
		text_1,
		text_2,
		text_3,
		text_all_draw,
		select,
		select_cursor,
		size,
	};

	static std::array<std::string, int(SoundName::size)> SoundNameStr = {
		"msp_bgm",
		"attack",
		"attack_hit",
		"jump",
		"avoid",
		"damage",
		"text_1",
		"text_2",
		"text_3",
		"text_all_draw",
		"select",
		"select_cursor",
	};
}