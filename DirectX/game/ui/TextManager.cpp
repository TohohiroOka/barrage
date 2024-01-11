#include "TextManager.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"

TextManager::TextManager()
{
}

TextManager::~TextManager()
{
}

void TextManager::Update()
{
	//テキストがセットされているならテキスト更新
	if (sentence.text) {
		SentenceNextText();
		sentence.text->Update();
	}

	//選択肢がセットされているなら選択肢更新
	if (choices.question) {
		choices.question->Update();
	}
}

void TextManager::Draw()
{
	//テキストがセットされているならテキスト描画
	if (sentence.text) {
		sentence.text->Draw();
	}

	//選択肢がセットされているなら選択肢更新
	if (choices.question) {
		choices.question->Draw();
	}
}

void TextManager::SentenceCreate(SentenceData::SentenceName sentenceName)
{
	//既に文章テキストが表示されていたら抜ける
	if (sentence.text) { return; }

	//テキスト表示回数をリセット
	textCount = 0;
	isSentenceEnd = false;

	//文章に文章名をセット
	sentence.sentenceName = sentenceName;
	//セットされた文章の最初のテキストを読み込み
	sentence.text = std::make_unique<TextTypeWriter>(TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]], textPos, textScale, textWriteSpeed);

	//テキスト表示回数を更新
	textCount++;
}

void TextManager::SentenceDrawEnd()
{
	//テキストを解放する
	sentence.text.reset();

	//テキスト表示回数をリセット
	textCount = 0;
	isSentenceEnd = false;
}

void TextManager::ChoicesCreate(ChoicesData::ChoicesName choicesName)
{
	//既に選択肢が表示されていたら抜ける
	if (choices.question) { return; }

	//文章に文章名をセット
	choices.choicesName = choicesName;
	//セットされた選択肢システムを読み込み
	choices.question = std::make_unique<QuestionSystem>(ChoicesData::choicesData[(int)choices.choicesName]);
}

void TextManager::ChoicesDrawEnd()
{
	//テキストを解放する
	choices.question.reset();
}

void TextManager::SentenceNextText()
{
	//テキストの表示が終了していなければ抜ける
	if (!sentence.text->GetIsAllWrite()) { return; }
	//入力がなければ抜ける
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_E) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A))) { return; }

	//文章のテキスト数に表示回数が達していたら終了フラグを立てて抜ける
	if (textCount >= (int)SentenceData::sentenceData[(int)sentence.sentenceName].size()) {
		isSentenceEnd = true;
		return;
	}

	//セットされている文章の次のテキストを読み込み
	sentence.text = std::make_unique<TextTypeWriter>(TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]], textPos, textScale, textWriteSpeed);

	//テキスト表示回数を更新
	textCount++;
}
