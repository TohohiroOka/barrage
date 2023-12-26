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
		main_before,
		bloom_before,
		outline_before,
		size,
	};

	enum class EngineUseRTVCreateTextureName {
		main_after,
		bloom_after,
		outline_after,
		fog_after,
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
		"main_before",
		"bloom_before",
		"outline_before",
	};

	static std::array<std::string, int(EngineUseRTVCreateTextureName::size)> EngineUseRTVCreateTexture = {
		"main_after",
		"bloom_after",
		"outline_after",
		"fog_after",
	};

	static std::array<std::string, int(EngineUseDSVTextureName::size)> EngineUseDSVTexture = {
		"shadowMap",
		"depth",
	};
}