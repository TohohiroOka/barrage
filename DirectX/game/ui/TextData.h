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
		TUTORIAL_JUMP_TEXT,			//�W�����v�������
	};

	//�e�L�X�g�f�[�^
	static std::vector<std::wstring> textData = {
		L"�Q�[�����J�n���܂����H",
		L"�`���[�g���A�����s���܂����H",
		L"�Q�[�����I�����܂����H",

		L"�`���[�g���A���ւ悤�����B�������ł͑�����@�̐������s���܂��B"
		L"�܂��́u�W�����v�v�ł��B����͂��邱�Ƃŏ�����ɒ��Ԃ��Ƃ��ł��܂��B"
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