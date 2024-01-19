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
	std::string textureName;
	if (XInputManager::GetInstance()->ControllerConnectCheck()) { textureName = "pad_" + std::to_string(XInputManager::PAD_A); }
	else { textureName = "key_" + std::to_string(DIK_SPACE); }
	inputNextTextSprite = Sprite::Create(textureName, { 1150, 775 }, { 0.5, 0.5f });
	inputNextTextSprite->SetScale(1.0f);

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
	if (sentence.text && !choices.question) {
		if (sentence.text->GetIsAllWrite()) {
			std::string textureName;
			if (XInputManager::GetInstance()->ControllerConnectCheck()) { textureName = "pad_" + std::to_string(XInputManager::PAD_A); }
			else { textureName = "key_" + std::to_string(DIK_SPACE); }
			inputNextTextSprite->SetTexture(textureName);
			inputNextTextSprite->SetScale(1.f);
			inputNextTextSprite->Update();
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
		sentence.text->Draw();
	}

	//テキストの表示が終了していれば入力スプライト描画
	if (sentence.text && !choices.question) {
		if (sentence.text->GetIsAllWrite()) {
			inputNextTextSprite->Draw();
		}
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
	sentence.text->Update();
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
	//テキストの表示が終了していなければ抜ける
	if (!sentence.text->GetIsAllWrite()) { return; }
	//入力がなければ抜ける
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_SPACE) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A))) { return; }

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
