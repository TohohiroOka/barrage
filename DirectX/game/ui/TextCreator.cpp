#include "TextCreator.h"

TextCreator::TextCreator(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, bool isStartDraw)
{
	//改行用カウント
	DirectX::XMINT2 posCounter = { 0, 0 };

	//テキストの文字数の分回す
	for (int i = 0; i < text.size(); i++) {
		//テキストから一文字ずつ取り出す
		const std::wstring character = text.substr(i, 1);

		//空白は飛ばす
		if (character == L"　") { continue; }

		//「＠」が検出されたら改行
		if (character == L"＠") {
			posCounter.x = 0;
			posCounter.y++;
			continue;
		}

		//文字スプライト生成
		DirectX::XMFLOAT2 pos = { leftTopPos.x + (CharSprite::charTextureSize - 2) * scale * posCounter.x, leftTopPos.y + (CharSprite::charTextureSize + 2) * posCounter.y };
		charSprites.push_back(std::make_unique<CharSprite>(character, pos, scale, isStartDraw));

		//次の文字を横にずらす
		posCounter.x++;
	}
}

TextCreator::~TextCreator()
{
}

void TextCreator::Update()
{
	for (auto& charSprite : charSprites) {
		charSprite->Update();
	}
}

void TextCreator::Draw()
{
	for (auto& charSprite : charSprites) {
		charSprite->Draw();
	}
}
