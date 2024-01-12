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
		GO_TO_GAME_CHECK_TEXT,		//�Q�[�����J�n���܂����H
		GO_TO_TUTORIAL_CHECK_TEXT,	//�`���[�g���A�����s���܂����H
		EXIT_GAME_CHECK_TEXT,		//�Q�[�����I�����܂����H
	};

	//�e�L�X�g�f�[�^
	static std::vector<std::wstring> textData = {
		L"�Q�[�����J�n���܂����H",
		L"�`���[�g���A�����s���܂����H",
		L"�Q�[�����I�����܂����H",
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
		GO_TO_GAME_CHECK,		//�Q�[�����J�n���邩�`�F�b�N
		GO_TO_TUTORIAL_CHECK,	//�`���[�g���A���s�����`�F�b�N
		EXIT_GAME_CHECK,		//�Q�[���I���`�F�b�N
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