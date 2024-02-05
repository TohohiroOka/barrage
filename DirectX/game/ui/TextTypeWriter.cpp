#include "TextTypeWriter.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Audio/Audio.h"
#include "GameHelper.h"

TextTypeWriter::TextTypeWriter(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, int writeWaitFrame)
	:TextCreator(text, leftTopPos, scale, false)
{
	//通常表示待機フレーム数をセット
	this->normalWriteWaitFrame = writeWaitFrame;
	//表示待機フレーム数をセット
	this->writeWaitFrame = normalWriteWaitFrame;

	//タイマー生成
	timer = std::make_unique<Engine::Timer>();
}

TextTypeWriter::~TextTypeWriter()
{
}

void TextTypeWriter::Update()
{
	//表示管理
	Write();
	AllWrite();

	//テキスト更新
	TextCreator::Update();

	//生成された瞬間を終了
	if (isCreateFrame) {
		isCreateFrame = false;
	}
}

void TextTypeWriter::Write()
{
	//全ての表示が終了していれば抜ける
	if (GetIsAllWrite()) { return; }

	//表示用タイマー更新
	timer->Update();
	//待機フレーム数に達していなければ抜ける
	if (*timer.get() < writeWaitFrame) { return; }

	//文字を表示状態にセット
	charSprites[writeCount]->SetIsDraw(true);
	//カタカタ音再生
	TextSound();

	//表示した文字が「、」や「。」などの場合はフレーム数を変更
	if (charSprites[writeCount]->GetCharacter() == L"、") {
		writeWaitFrame = commaWaitFrame;

		//カタカタ音の種類を初期の大きい音に戻す
		soundNumber = 0;
	}
	else if (charSprites[writeCount]->GetCharacter() == L"。") {
		writeWaitFrame = periodWaitFrame;

		//カタカタ音の種類を初期の大きい音に戻す
		soundNumber = 0;
	}
	else {
		writeWaitFrame = normalWriteWaitFrame;
	}

	//タイマーリセット
	timer->Reset();

	//次の文字番号に更新
	writeCount++;
}

void TextTypeWriter::TextSound()
{
	//カタカタ音再生
	if (soundNumber == 0) { Audio::Instance()->SoundPlayWava(Sound::SoundName::text_1, false, 0.1f); }
	else if (soundNumber == 1) { Audio::Instance()->SoundPlayWava(Sound::SoundName::text_2, false, 0.1f); }
	else if (soundNumber == 2) { Audio::Instance()->SoundPlayWava(Sound::SoundName::text_3, false, 0.1f); }

	//次に鳴らす音を設定
	int randamNum = RandomInt(1);
	if (randamNum == 0) {
		if (soundNumber == 0) { soundNumber = 1; }
		else if (soundNumber == 1) { soundNumber = 2; }
		else if (soundNumber == 2) { soundNumber = 0; }
	}
	else if (randamNum == 1) {
		if (soundNumber == 0) { soundNumber = 2; }
		else if (soundNumber == 1) { soundNumber = 0; }
		else if (soundNumber == 2) { soundNumber = 1; }
	}
}

void TextTypeWriter::AllWrite()
{
	//全ての表示が終了していれば抜ける
	if (GetIsAllWrite()) { return; }
	//入力がなければ抜ける
	if (!GameInputManager::TriggerInputAction(GameInputManager::Select)) { return; }
	//生成された瞬間なら抜ける
	if (isCreateFrame) { return; }

	//表示されていない文字を一気に全表示する
	for (int i = writeCount; i < GetTextLength(); i++) {
		charSprites[i]->SetIsDraw(true);
	}

	//テキスト全表示音再生
	Audio::Instance()->SoundPlayWava(Sound::SoundName::text_all_draw, false, 0.1f);
}
