#pragma once
#include <array>

namespace Sound {
	enum class SoundName {
		msp_bgm,
		size,
	};

	static std::array<std::string, int(SoundName::size)> SoundNameStr = {
		"msp_bgm"
	};
}