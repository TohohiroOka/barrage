#pragma once
#include <vector>
#include <string>

/// <summary>
/// ゲームで使用するテキストデータ
/// </summary>
namespace TextData {
	/// <summary>
	/// テキストデータ名
	/// </summary>
	enum class TextName {
		GO_TO_GAME_CHECK_TEXT,		//ゲームを開始しますか？
		GO_TO_TUTORIAL_CHECK_TEXT,	//チュートリアルを行いますか？
		EXIT_GAME_CHECK_TEXT,		//ゲームを終了しますか？
	};

	//テキストデータ
	static std::vector<std::wstring> textData = {
		L"ゲームを開始しますか？",
		L"チュートリアルを行いますか？",
		L"ゲームを終了しますか？",
	};
}

/// <summary>
/// ゲームで使用する文章データ
/// </summary>
namespace SentenceData {
	/// <summary>
	/// 文章名
	/// </summary>
	enum class SentenceName
	{
		GO_TO_GAME_CHECK,		//ゲームを開始するかチェック
		GO_TO_TUTORIAL_CHECK,	//チュートリアル行くかチェック
		EXIT_GAME_CHECK,		//ゲーム終了チェック
	};

	//文章データ
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
/// ゲームで使用する選択肢データ
/// </summary>
namespace ChoicesData {
	/// <summary>
	/// 選択肢データ名
	/// </summary>
	enum class ChoicesName {
		YES_OR_NO,		//はいorいいえ
		KANSAI,			//なんでやねんorアホかorどつきまわすぞ
	};


	static std::vector<std::vector<std::wstring>> choicesData = {
		{
			L"はい",
			L"いいえ"
		},
		{
			L"なんでやねん",
			L"アホか",
			L"どつきまわすぞ",
		}
	};
}