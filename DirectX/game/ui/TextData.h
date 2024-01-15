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
		//タイトルシーン用
		GO_TO_GAME_CHECK_TEXT,		//ゲームを開始しますか？
		GO_TO_TUTORIAL_CHECK_TEXT,	//チュートリアルを行いますか？
		EXIT_GAME_CHECK_TEXT,		//ゲームを終了しますか？


		//チュートリアルシーン用
		TUTORIAL_START_TEXT,		//チュートリアルへようこそ。＠ここでは操作方法とルールの説明を行います。
		TUTORIAL_JUMP_TEXT,			//ジャンプ操作説明
	};

	//テキストデータ
	static std::vector<std::wstring> textData = {
		L"ゲームを開始しますか？",
		L"チュートリアルを行いますか？",
		L"ゲームを終了しますか？",

		L"チュートリアルへようこそ。＠ここでは操作方法の説明を行います。"
		L"まずは「ジャンプ」です。を入力することで上方向に跳ぶことができます。"
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
		//タイトルシーン用
		GO_TO_GAME_CHECK,		//ゲームを開始するかチェック
		GO_TO_TUTORIAL_CHECK,	//チュートリアル行くかチェック
		EXIT_GAME_CHECK,		//ゲーム終了チェック


		//チュートリアルシーン用
		TUTORIAL_START,		//チュートリアル開始
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
		{
			TextData::TextName::TUTORIAL_START_TEXT,
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