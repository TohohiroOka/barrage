#pragma once
#include <array>

namespace Sound {
	enum class SoundName {
		bgm,
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
		boss_nearAttack_sword,
		bullet1,
		bullet1_start,
		sword_fly,
		bullet2_start,
		screen_break1,
		screen_break2,
		half_attack,
		near_attack,
		boss_slash,
		size,
	};

	static std::array<std::string, int(SoundName::size)> SoundNameStr = {
		"bgm",
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
		"boss_nearAttack_sword",//近づく攻撃用意
		"bullet1",//bullet1の発射音
		"bullet1_start",//bullet1の開始音
		"sword_fly",//剣を飛ばす系の攻撃音
		"bullet2_start",//bullet2の開始音
		"screen_break1",//画面破壊エフェクト音
		"screen_break2",//画面破壊エフェクト音
		"half_attack",//中心に集まってオルガノン
		"near_attack",//地面からとげ
		"boss_slash"//近づいてからの攻撃音
	};
}