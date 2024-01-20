#pragma once
#include <vector>
#include <string>

/// <summary>
/// �Q�[���Ŏg�p����e�L�X�g�f�[�^
/// </summary>
namespace TextData {
	/// <summary>
	/// �e�L�X�g�f�[�^��
	/// </summary>
	enum class TextName {
		//�^�C�g���V�[���p
		GO_TO_GAME_CHECK_TEXT,		//�Q�[�����J�n���܂����H
		GO_TO_TUTORIAL_CHECK_TEXT,	//�`���[�g���A�����s���܂����H
		EXIT_GAME_CHECK_TEXT,		//�Q�[�����I�����܂����H


		//�`���[�g���A���V�[���p
		TUTORIAL_START_TEXT,		//�`���[�g���A���ւ悤�����B�������ł͑�����@�ƃ��[���̐������s���܂��B

		TUTORIAL_RUN_TEXT,			//�܂��́u�_�b�V���ړ��v�ł��B���ړ����Ȃ���[action_input_dash]����͂��邱�Ƃő����ړ��ł��܂��B
		TUTORIAL_RUN_ACTION_TEXT,	//�_�b�V���F�ړ����Ȃ���[action_input_dash]�������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@����(1000)��
		
		TUTORIAL_JUMP_TEXT,			//���́u�W�����v�v�ł��B��[action_input_jump]����͂��邱�Ƃŏ�����ɒ��Ԃ��Ƃ��ł��܂��B
		TUTORIAL_JUMP_ACTION_TEXT,	//�W�����v�F[action_input_dash]�������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@����(5)��

		TUTORIAL_ATTACK_TEXT_1,		//���́u�W�����v�v�ł��B��[action_input_jump]����͂��邱�Ƃŏ�����ɒ��Ԃ��Ƃ��ł��܂��B
		TUTORIAL_ATTACK_TEXT_2,		//���́u�W�����v�v�ł��B��[action_input_jump]����͂��邱�Ƃŏ�����ɒ��Ԃ��Ƃ��ł��܂��B
		TUTORIAL_ATTACK_ACTION_TEXT,//�W�����v�F[action_input_dash]�������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@����(5)��

		TUTORIAL_AVOID_TEXT,			//���́u�W�����v�v�ł��B��[action_input_jump]����͂��邱�Ƃŏ�����ɒ��Ԃ��Ƃ��ł��܂��B
		TUTORIAL_AVOID_ACTION_TEXT,	//�W�����v�F[action_input_dash]������

		TUTORIAL_FREE_TEXT,			//���́u�W�����v�v�ł��B��[action_input_jump]����͂��邱�Ƃŏ�����ɒ��Ԃ��Ƃ��ł��܂��B
		TUTORIAL_FREE_ACTION_TEXT,	//�W�����v�F[action_input_dash]������
	};

	struct TextDataContents {
		std::wstring text;
		bool isTypeWriter;
		bool isInputNextText;
	};

	//�e�L�X�g�f�[�^
	static std::vector<TextDataContents> textData = {
		{ L"�Q�[�����J�n���܂����H", true, true },
		{ L"�`���[�g���A�����s���܂����H", true, true },
		{ L"�Q�[�����I�����܂����H", true, true },
		

		{ L"�`���[�g���A���ւ悤�����B�������ł͑�����@�̐������s���܂��B", true, true },

		{ L"�܂��́u�_�b�V���ړ��v�ł��B���ړ����Ȃ���[action_input_dash]����͂��邱�Ƃő����ړ��ł��܂��B", true, true },
		{ L"�_�b�V���F�ړ����Ȃ���[action_input_dash]�������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@����(100)��", false, false },

		{ L"���́u�W�����v�v�ł��B��[action_input_jump]����͂��邱�Ƃŏ�����ɒ��Ԃ��Ƃ��ł��܂��B", true, true },
		{ L"�W�����v�F[action_input_jump]�������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@����(5)��", false, false },

		{ L"�����Ắu�U���v�ł��B���U���ɂ͎�U���Ƌ��U����2��ނ�����܂��B", true, true },
		{ L"[action_input_light_attack]����͂��邱�ƂŎ�U���A[action_input_strong_attack]����͂��邱�ƂŁ����U�����o�����Ƃ��ł��܂��B���ǂ�����ő�3��܂ŘA�������邱�Ƃ��ł��܂��B", true, true },
		{ L"��U���F[action_input_light_attack]�����U���F[action_input_strong_attack]�����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@����(1)��", false, false },

		{ L"�Ō�́u����v�ł��B��[action_input_avoid]����͂��邱�ƂœG�̍U��������邱�Ƃ��ł��܂��B", true, true },
		{ L"����F[action_input_avoid]�������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@����(3)��", false, false },

		{ L"�ȏ�ő�������͏I���ł��B�����R�ɍs�����Ă݂܂��傤�B", true, true },
		{ L"���R�ɍs�����Ă݂悤�B�������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@[action_input_select]�F�|�[�Y", false, false },
	};
}

/// <summary>
/// �Q�[���Ŏg�p���镶�̓f�[�^
/// </summary>
namespace SentenceData {
	/// <summary>
	/// ���͖�
	/// </summary>
	enum class SentenceName
	{
		//�^�C�g���V�[���p
		GO_TO_GAME_CHECK,		//�Q�[�����J�n���邩�`�F�b�N
		GO_TO_TUTORIAL_CHECK,	//�`���[�g���A���s�����`�F�b�N
		EXIT_GAME_CHECK,		//�Q�[���I���`�F�b�N


		//�`���[�g���A���V�[���p
		TUTORIAL_START,		//�`���[�g���A���J�n
		TUTORIAL_RUN,		//�_�b�V���������
		TUTORIAL_JUMP,		//�W�����v�������
		TUTORIAL_ATTACK,
		TUTORIAL_AVOID,
		TUTORIAL_FREE
	};

	//���̓f�[�^
	static std::vector<std::vector<TextData::TextName>> sentenceData = {
		{
			TextData::TextName::GO_TO_GAME_CHECK_TEXT,
		},
		{
			TextData::TextName::GO_TO_TUTORIAL_CHECK_TEXT,
		},
		{
			TextData::TextName::EXIT_GAME_CHECK_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_START_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_RUN_TEXT,
			TextData::TextName::TUTORIAL_RUN_ACTION_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_JUMP_TEXT,
			TextData::TextName::TUTORIAL_JUMP_ACTION_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_ATTACK_TEXT_1,
			TextData::TextName::TUTORIAL_ATTACK_TEXT_2,
			TextData::TextName::TUTORIAL_ATTACK_ACTION_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_AVOID_TEXT,
			TextData::TextName::TUTORIAL_AVOID_ACTION_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_FREE_TEXT,
			TextData::TextName::TUTORIAL_FREE_ACTION_TEXT,
		},
	};
}

/// <summary>
/// �Q�[���Ŏg�p����I�����f�[�^
/// </summary>
namespace ChoicesData {
	/// <summary>
	/// �I�����f�[�^��
	/// </summary>
	enum class ChoicesName {
		YES_OR_NO,		//�͂�or������
		KANSAI,			//�Ȃ�ł�˂�or�A�z��or�ǂ��܂킷��
	};


	static std::vector<std::vector<std::wstring>> choicesData = {
		{
			L"�͂�",
			L"������"
		},
		{
			L"�Ȃ�ł�˂�",
			L"�A�z��",
			L"�ǂ��܂킷��",
		}
	};
}