#include "TextManager.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "WindowApp.h"
#include "Math/Easing/Easing.h"

TextManager::TextManager()
{
	//枠スプライトを生成
	const DirectX::XMFLOAT2 framePos = { (float)WindowApp::GetWindowWidth() / 2, 700 };
	sentenceFrameSprite = Sprite::Create("white", framePos, { 0.5f, 0.5f }, { 0.1f, 0.1f, 0.1f, 0 });
	DirectX::XMFLOAT2 backSize = { 900, 220 };
	sentenceFrameSprite->SetSize(backSize);

	//次のテキストを生成する入力スプライトを生成
	inputNextTextSprite = std::make_unique<CharActionInputSprite>(GameInputManager::InputAction::Select, DirectX::XMFLOAT2{ 1150, 780 }, 1.0f, true);

	//各状態の内容をセット
	func.emplace_back([this] { return UpdateSentenceNonePhase(); });
	func.emplace_back([this] { return UpdateSentenceFrameEmergePhase(); });
	func.emplace_back([this] { return UpdateSentenceCreatePhase(); });
	func.emplace_back([this] { return UpdateSentenceFrameSubemergePhase(); });

	//タイマークラス
	easeTimer = std::make_unique<Engine::Timer>();
}

TextManager::~TextManager()
{
}

void TextManager::Update()
{
	//枠スプライト更新
	if (!(sentenceCreatePhase == SentenceCreatePhase::NONE)) {
		sentenceFrameSprite->Update();
	}
	//入力スプライト描画
	if (sentence.textCreator && !choices.question) {
		//テキストごとの「入力によるテキスト更新フラグ」がtrueなら
		if (isInputNextText) {
			if (sentence.textCreator->GetIsAllWrite()) {
				inputNextTextSprite->Update();
			}
		}
	}

	//文章の各状態の内容を更新
	func[int(sentenceCreatePhase)]();

	//選択肢がセットされているなら選択肢更新
	if (choices.question) {
		choices.question->Update();
	}
}

void TextManager::Draw()
{
	//枠スプライト描画
	if (!(sentenceCreatePhase == SentenceCreatePhase::NONE)) {
		sentenceFrameSprite->Draw();
	}

	//文章生成状態ならテキスト描画
	if (sentenceCreatePhase == SentenceCreatePhase::SENTENCE_CREATE) {
		sentence.textCreator->Draw();
	}

	//テキストの表示が終了していれば入力スプライト描画
	if (sentence.textCreator && !choices.question) {
		//テキストごとの「入力によるテキスト更新フラグ」がtrueなら
		if (isInputNextText) {
			if (sentence.textCreator->GetIsAllWrite()) {
				inputNextTextSprite->Draw();
			}
		} 
	}

	//選択肢がセットされているなら選択肢更新
	if (choices.question) {
		choices.question->Draw();
	}
}

void TextManager::SentenceCreate(SentenceData::SentenceName sentenceName)
{
	//テキストの表示が終了していれば
	if (!isSentenceEnd && sentence.textCreator->GetIsAllWrite()) {
		//文章のテキスト数に表示回数が達していたら終了フラグを立てる
		if (textCount >= (int)SentenceData::sentenceData[(int)sentence.sentenceName].size()) {
			isSentenceEnd = true;
		}
	}

	//それでも文章表示し終えていなければ抜ける
	if (!isSentenceEnd) { return; }

	//テキスト表示回数をリセット
	textCount = 0;
	isSentenceEnd = false;

	//表示を終えた文章が表示されたままなら、新たな文章を入れるため文章のテキストを解放しておく
	if (sentence.textCreator) {
		sentence.textCreator.reset();
	}

	//文章に文章名をセット
	sentence.sentenceName = sentenceName;
	//セットされた文章の最初のテキストを読み込み
	sentence.text = TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].text;
	//入力によるテキスト更新を可能にするか
	isInputNextText = TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].isInputNextText;

	//テキストごとに指定した、タイプライターシステムを使用するか判定
	if (TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].isTypeWriter) {
		sentence.textCreator = std::make_unique<TextTypeWriter>(sentence.text, textPos, textScale, textWriteSpeed);
	}
	else {
		sentence.textCreator = std::make_unique<TextCreator>(sentence.text, textPos, textScale, true);
	}

	//テキスト表示回数を更新
	textCount++;

	//文章生成状態がなにもないフェーズなら枠が出てくるフェーズに更新
	if (sentenceCreatePhase == SentenceCreatePhase::NONE) {
		sentenceCreatePhase = SentenceCreatePhase::FRAME_EMERGE;
	}
}

void TextManager::SentenceDrawEnd()
{
	//枠スプライトを消滅させる
	sentenceCreatePhase = SentenceCreatePhase::FRAME_SUBMERGE;

	//テキストを解放する
	sentence.textCreator.reset();

	//テキスト表示回数をリセット
	textCount = 0;
	isSentenceEnd = true;
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

void TextManager::Finalize()
{
	sentenceFrameSprite.reset();
	inputNextTextSprite.reset();
}

void TextManager::UpdateSentenceNonePhase()
{
	//なにもしないフェーズ
}

void TextManager::UpdateSentenceFrameEmergePhase()
{
	//枠スプライト出現にかかる時間
	const float emergeTime = 10;
	easeTimer->Update();
	//イージング用タイム割合を算出
	const float time = *easeTimer.get() / emergeTime;

	//色の透明度をイージングで変更
	DirectX::XMFLOAT4 color = sentenceFrameSprite->GetColor();
	color.w = Easing::Lerp(0, frameColorAlpha, time);
	sentenceFrameSprite->SetColor(color);

	//タイマーが指定した時間になったら文章を生成する状態に変更
	if (*easeTimer.get() >= emergeTime) {
		sentenceCreatePhase = SentenceCreatePhase::SENTENCE_CREATE;

		//タイマーをリセットしておく
		easeTimer->Reset();
	}
}

void TextManager::UpdateSentenceCreatePhase()
{
	//次のテキストがあるなら更新
	SentenceNextText();

	//スプライト更新
	sentence.textCreator->Update();
}

void TextManager::UpdateSentenceFrameSubemergePhase()
{
	//枠スプライト消滅にかかる時間
	const float submergeTime = 10;
	easeTimer->Update();
	//イージング用タイム割合を算出
	const float time = *easeTimer.get() / submergeTime;

	//色の透明度をイージングで変更
	DirectX::XMFLOAT4 color = sentenceFrameSprite->GetColor();
	color.w = Easing::Lerp(frameColorAlpha, 0, time);
	sentenceFrameSprite->SetColor(color);

	//タイマーが指定した時間になったら文章をなにもしない状態に変更
	if (*easeTimer.get() >= submergeTime) {
		sentenceCreatePhase = SentenceCreatePhase::NONE;

		//タイマーをリセットしておく
		easeTimer->Reset();
	}
}

void TextManager::SentenceNextText()
{
	//テキストごとの「入力によるテキスト更新フラグ」がfalseなら抜ける
	if (!isInputNextText) { return; }

	//テキストの表示が終了していなければ抜ける
	if (!sentence.textCreator->GetIsAllWrite()) { return; }
	//入力がなければ抜ける
	if (!GameInputManager::TriggerInputAction(GameInputManager::Select)) { return; }

	//文章のテキスト数に表示回数が達していたら終了フラグを立てて抜ける
	if (textCount >= (int)SentenceData::sentenceData[(int)sentence.sentenceName].size()) {
		isSentenceEnd = true;
		return;
	}

	//テキストを更新
	//セットされている文章の次のテキストを読み込み
	sentence.text = TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].text;
	//入力によるテキスト更新を可能にするか
	isInputNextText = TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].isInputNextText;

	//テキストごとに指定した、タイプライターシステムを使用するか判定
	if (TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].isTypeWriter) {
		sentence.textCreator = std::make_unique<TextTypeWriter>(sentence.text, textPos, textScale, textWriteSpeed);
	}
	else {
		sentence.textCreator = std::make_unique<TextCreator>(sentence.text, textPos, textScale, true);
	}

	//テキスト表示回数を更新
	textCount++;
}
