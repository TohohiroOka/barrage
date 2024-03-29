#pragma once
#include "TextTypeWriter.h"
#include "QuestionSystem.h"
#include "TextData.h"
#include "Singleton.h"
#include "CharActionInputSprite.h"

/// <summary>
/// テキスト管理
/// </summary>
class TextManager : public Singleton<TextManager>
{
	friend class Singleton<TextManager>;

public: //enum
	/// <summary>
	/// 文章生成のフェーズ
	/// </summary>
	enum class SentenceCreatePhase
	{
		NONE,			//文章がなにもないフェーズ
		FRAME_EMERGE,	//文章を囲う枠が出てくるフェーズ
		SENTENCE_CREATE,//文章を生成するフェーズ
		FRAME_SUBMERGE,	//文章を囲う枠が消えていくフェーズ
	};

public: //構造体
	/// <summary>
	/// 文章
	/// </summary>
	struct Sentence
	{
		std::unique_ptr<TextCreator> textCreator; //テキスト表示
		std::wstring text;//テキスト
		SentenceData::SentenceName sentenceName; //文章名
	};

	/// <summary>
	/// 選択肢
	/// </summary>
	struct Choices
	{
		std::unique_ptr<QuestionSystem> question; //選択肢システム
		ChoicesData::ChoicesName choicesName; //選択肢名
	};

public: //メンバ関数
	TextManager();
	~TextManager();

	/// <summary>
	/// 文章更新
	/// </summary>
	void UpdateSentence();

	/// <summary>
	/// 選択肢更新
	/// </summary>
	void UpdateChoices();

	/// <summary>
	/// 文章描画
	/// </summary>
	void DrawSentence();

	/// <summary>
	/// 選択肢描画
	/// </summary>
	void DrawChoices();

	/// <summary>
	/// 文章生成
	/// </summary>
	/// <param name="sentenceName">文章名</param>
	void SentenceCreate(SentenceData::SentenceName sentenceName);

	/// <summary>
	/// 文章の表示終了
	/// </summary>
	void SentenceDrawEnd();


	/// <summary>
	/// 選択肢生成
	/// </summary>
	/// <param name="choicesName">選択肢名</param>
	void ChoicesCreate(ChoicesData::ChoicesName choicesName);

	/// <summary>
	/// 選択肢の表示終了
	/// </summary>
	void ChoicesDrawEnd();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	//getter
	const Sentence& GetSentece() { return sentence; }
	bool GetIsSentenceEnd(SentenceData::SentenceName sentenceName) {
		//文章名が違っていたらfalse
		if (!(sentence.sentenceName == sentenceName)) { return false; }

		return isSentenceEnd;
	}
	const Choices& GetChoices() { return choices; }
	bool GetIsChoiceEnd() {
		//そもそも選択肢が表示されていなければfalse
		if (!choices.question) { return false; }
		return choices.question->GetIsEnd(); 
	}
	int GetSelectNum(ChoicesData::ChoicesName choicesName) {
		//選択肢名が違っていたらどでかい数字を返す
		if (!(choices.choicesName == choicesName)) { return 1000; }

		return choices.question->GetSelectNum();
	}

private: //メンバ関数
	//角文章フェーズの更新処理
	void UpdateSentenceNonePhase();
	void UpdateSentenceFrameEmergePhase();
	void UpdateSentenceCreatePhase();
	void UpdateSentenceFrameSubemergePhase();


	/// <summary>
	/// 次のテキストを表示
	/// </summary>
	void SentenceNextText();

private: //定数
	//テキスト1文字目の座標
	const DirectX::XMFLOAT2 textPos = { 400, 640 };
	//テキストの大きさ
	const float textScale = 1.0f;
	//テキストを表示する早さ
	const int textWriteSpeed = 2;
	//枠スプライトのalpha
	const float frameColorAlpha = 0.7f;

private: //メンバ変数
	//文章生成のフェーズ
	SentenceCreatePhase sentenceCreatePhase = SentenceCreatePhase::NONE;
	//各フェーズの動き
	std::vector<std::function<void()>> func;
	//イージング用タイマー
	std::unique_ptr<Engine::Timer> easeTimer;

	//文章を囲う枠スプライト
	std::unique_ptr<Sprite> sentenceFrameSprite;

	//次のテキストを表示させるためのボタンスプライト
	std::unique_ptr<CharActionInputSprite> inputNextTextSprite;

	//文章
	Sentence sentence;
	//1文章のテキスト更新回数
	int textCount = 0;
	//文章を表示し終えたか
	bool isSentenceEnd = true;
	//テキスト更新入力を可能にするか
	bool isInputNextText = true;

	//選択肢
	Choices choices;
};