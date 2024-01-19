#include "TextTypeWriter.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"

TextTypeWriter::TextTypeWriter(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, int writeWaitFrame)
{
	//文章生成
	this->text = std::make_unique<TextCreator>(text, leftTopPos, scale, false);

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

	text->Update();

	//生成された瞬間を終了
	if (isCreateFrame) {
		isCreateFrame = false;
	}
}

void TextTypeWriter::Draw()
{
	text->Draw();
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
	text->GetCharSprite(writeCount)->SetIsDraw(true);

	//表示した文字が「、」や「。」などの場合はフレーム数を変更
	if (text->GetCharSprite(writeCount)->GetCharacter() == L"、") {
		writeWaitFrame = commaWaitFrame;
	}
	else if (text->GetCharSprite(writeCount)->GetCharacter() == L"。") {
		writeWaitFrame = periodWaitFrame;
	}
	else {
		writeWaitFrame = normalWriteWaitFrame;
	}

	//タイマーリセット
	timer->Reset();

	//次の文字番号に更新
	writeCount++;
}

void TextTypeWriter::AllWrite()
{
	//全ての表示が終了していれば抜ける
	if (GetIsAllWrite()) { return; }
	//入力がなければ抜ける
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_SPACE) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A))) { return; }
	//生成された瞬間なら抜ける
	if (isCreateFrame) { return; }

	//表示されていない文字を一気に全表示する
	for (int i = writeCount; i < text->GetTextLength(); i++) {
		text->GetCharSprite(i)->SetIsDraw(true);
	}
}
