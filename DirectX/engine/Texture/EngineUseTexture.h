#pragma once
#include <string>
#include <array>

namespace Engine {
	enum class EngineUseLoadTextureName {
		debugfont,
		white,
		size,
	};

	enum class EngineUseRTVTextureName {
		main,
		bloom,
		outline,
		fog,
		size,
	};

	enum class EngineUseDSVTextureName {
		shadowMap,
		depth,
		size,
	};

	static std::array<std::string, int(EngineUseLoadTextureName::size)> EngineUseLoadTexture = {
		"LetterResources/debugfont.png",
		"SubTexture/white1x1.png"
	};

	static std::array<std::string, int(EngineUseRTVTextureName::size)> EngineUseRTVTexture = {
		"main",
		"bloom",
		"outline",
		"fog",
	};

	static std::array<std::string, int(EngineUseDSVTextureName::size)> EngineUseDSVTexture = {
		"shadowMap",
		"depth",
	};
}