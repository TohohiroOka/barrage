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
		"boss_nearAttack_sword",//�߂Â��U���p��
		"bullet1",//bullet1�̔��ˉ�
		"bullet1_start",//bullet1�̊J�n��
		"sword_fly",//�����΂��n�̍U����
		"bullet2_start",//bullet2�̊J�n��
		"screen_break1",//��ʔj��G�t�F�N�g��
		"screen_break2",//��ʔj��G�t�F�N�g��
		"half_attack",//���S�ɏW�܂��ăI���K�m��
		"near_attack",//�n�ʂ���Ƃ�
		"boss_slash"//�߂Â��Ă���̍U����
	};
}