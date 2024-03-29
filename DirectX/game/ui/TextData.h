﻿#pragma once
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

		TUTORIAL_RUN_TEXT,			//まずは「ダッシュ移動」です。＠移動しながら[action_input_dash]を入力することで速く移動できます。
		TUTORIAL_RUN_ACTION_TEXT,	//ダッシュ：移動しながら[action_input_dash]＠＠＠　　　　　　　　　　　　　　　　あと(1000)ｍ
		
		TUTORIAL_JUMP_TEXT,			//つぎは「ジャンプ」です。＠[action_input_jump]を入力することで上方向に跳ぶことができます。
		TUTORIAL_JUMP_ACTION_TEXT,	//ジャンプ：[action_input_dash]＠＠＠　　　　　　　　　　　　　　　　あと(5)回

		TUTORIAL_CAMERA_TEXT_1_KEY,	//
		TUTORIAL_CAMERA_TEXT_1_PAD,	//
		TUTORIAL_CAMERA_TEXT_2,		//
		TUTORIAL_CAMERA_TEXT_3,		//
		TUTORIAL_CAMERA_TEXT_4,		//
		TUTORIAL_CAMERA_ACTION_TEXT,//

		TUTORIAL_ATTACK_TEXT_1,		//つぎは「ジャンプ」です。＠[action_input_jump]を入力することで上方向に跳ぶことができます。
		TUTORIAL_ATTACK_TEXT_2,		//つぎは「ジャンプ」です。＠[action_input_jump]を入力することで上方向に跳ぶことができます。
		TUTORIAL_ATTACK_TEXT_3,		//つぎは「ジャンプ」です。＠[action_input_jump]を入力することで上方向に跳ぶことができます。
		TUTORIAL_ATTACK_ACTION_TEXT,//ジャンプ：[action_input_dash]＠＠＠　　　　　　　　　　　　　　　　あと(5)回

		TUTORIAL_AVOID_TEXT_1,		//つぎは「ジャンプ」です。＠[action_input_jump]を入力することで上方向に跳ぶことができます。
		TUTORIAL_AVOID_TEXT_2,		//つぎは「ジャンプ」です。＠[action_input_jump]を入力することで上方向に跳ぶことができます。
		TUTORIAL_AVOID_ACTION_TEXT,	//ジャンプ：[action_input_dash]＠＠＠

		TUTORIAL_FREE_TEXT,			//つぎは「ジャンプ」です。＠[action_input_jump]を入力することで上方向に跳ぶことができます。
		TUTORIAL_FREE_ACTION_TEXT,	//ジャンプ：[action_input_dash]＠＠＠
	};

	struct TextDataContents {
		std::wstring text;
		bool isTypeWriter;
		bool isInputNextText;
	};

	//テキストデータ
	static std::vector<TextDataContents> textData = {
		{ L"ゲームを開始しますか？", true, true },
		{ L"チュートリアルを行いますか？", true, true },
		{ L"ゲームを終了しますか？", true, true },
		

		{ L"チュートリアルへようこそ。＠ここでは操作方法の説明を行います。", true, true },

		{ L"まずは「ダッシュ移動」です。＠[action_input_dash]を入力しながら移動することで速く移動できます。", true, true },
		{ L"ダッシュ：[action_input_dash]を入力しながら移動＠＠＠　　　　　　　　　　　　　　　　　あと(300)ｍ", false, false },

		{ L"つぎは「ジャンプ」です。＠[action_input_jump]を入力することで上方向に跳ぶことができます。", true, true },
		{ L"ジャンプ：[action_input_jump]＠＠＠　　　　　　　　　　　　　　　　　　　あと(5)回", false, false },

		{ L"つぎは「カメラ操作」です。＠[action_input_camera_up_rota][action_input_camera_left_rota][action_input_camera_down_rota][action_input_camera_right_rota]入力でカメラ回転ができます。", true, true },
		{ L"つぎは「カメラ操作」です。＠[action_input_camera_up_rota]でカメラ回転ができます。", true, true },
		{ L"敵がいる場合は、＠[action_input_camera_lockon]で「ロックオン」ができます。", true, true },
		{ L"おや・・・・・？", true, true },
		{ L"ちょうどいいタイミングで敵が出てきました！＠敵をロックオンしてみましょう。", true, true },
		{ L"ロックオン：敵が画面内にいるときに[action_input_camera_lockon]＠＠＠　　　　　　　　　　　　　　　　　　　あと(1)回", false, false },

		{ L"続いては「攻撃」です。＠攻撃には弱攻撃と強攻撃の2種類があります。", true, true },
		{ L"[action_input_light_attack]を入力することで弱攻撃、[action_input_strong_attack]を入力することで＠強攻撃を出すことができます。＠どちらも最大3回まで連撃をすることができます。", true, true },
		{ L"それでは敵に攻撃を当ててみましょう。", true, true },
		{ L"弱攻撃：[action_input_light_attack]＠強攻撃：[action_input_strong_attack]＠＠　　　　　　　　　　　　　　　　　　あと(10)回", false, false },

		{ L"最後は「回避」です。＠移動しながら[action_input_avoid]を入力することで、敵の攻撃を＠避けることができます。", true, true },
		{ L"敵の攻撃を引き付けてから、回避で避けてみましょう。", true, true },
		{ L"回避：移動しながら[action_input_avoid]＠＠＠　　　　　　　　　　　　　　　　　　　あと(3)回", false, false },

		{ L"以上で操作説明は終了です。＠自由に行動してみましょう。", true, true },
		{ L"自由に行動してみよう。＠＠＠　　　　　　　　　　　　　　　　　　　[action_input_pause]：ポーズ", false, false },
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
		TUTORIAL_RUN,		//ダッシュ操作説明
		TUTORIAL_JUMP,		//ジャンプ操作説明
		TUTORIAL_CAMERA_1_KEY,
		TUTORIAL_CAMERA_1_PAD,
		TUTORIAL_CAMERA_2,
		TUTORIAL_ATTACK,
		TUTORIAL_AVOID,
		TUTORIAL_FREE,
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
		{
			TextData::TextName::TUTORIAL_RUN_TEXT,
			TextData::TextName::TUTORIAL_RUN_ACTION_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_JUMP_TEXT,
			TextData::TextName::TUTORIAL_JUMP_ACTION_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_CAMERA_TEXT_1_KEY,
			TextData::TextName::TUTORIAL_CAMERA_TEXT_2,
			TextData::TextName::TUTORIAL_CAMERA_TEXT_3,
		},
		{
			TextData::TextName::TUTORIAL_CAMERA_TEXT_1_PAD,
			TextData::TextName::TUTORIAL_CAMERA_TEXT_2,
			TextData::TextName::TUTORIAL_CAMERA_TEXT_3,
		},
		{
			TextData::TextName::TUTORIAL_CAMERA_TEXT_4,
			TextData::TextName::TUTORIAL_CAMERA_ACTION_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_ATTACK_TEXT_1,
			TextData::TextName::TUTORIAL_ATTACK_TEXT_2,
			TextData::TextName::TUTORIAL_ATTACK_TEXT_3,
			TextData::TextName::TUTORIAL_ATTACK_ACTION_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_AVOID_TEXT_1,
			TextData::TextName::TUTORIAL_AVOID_TEXT_2,
			TextData::TextName::TUTORIAL_AVOID_ACTION_TEXT,
		},
		{
			TextData::TextName::TUTORIAL_FREE_TEXT,
			TextData::TextName::TUTORIAL_FREE_ACTION_TEXT,
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
		TUTORIAL_PAUSE_CHOICE,
		KANSAI,			//なんでやねんorアホかorどつきまわすぞ
	};


	static std::vector<std::vector<std::wstring>> choicesData = {
		{
			L"はい",
			L"いいえ"
		},
		{
			L"チュートリアルをつづける",
			L"ゲームを開始する",
			L"タイトルにもどる",
		},
		{
			L"なんでやねん",
			L"アホか",
			L"どつきまわすぞ",
		}
	};
}