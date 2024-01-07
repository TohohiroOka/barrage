#include "TextCreator.h"

TextCreator::TextCreator(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, bool isStartDraw)
{
	//���s�p�J�E���g
	DirectX::XMINT2 posCounter = { 0, 0 };

	//�e�L�X�g�̕������̕���
	for (int i = 0; i < text.size(); i++) {
		//�e�L�X�g����ꕶ�������o��
		const std::wstring character = text.substr(i, 1);

		//�󔒂͔�΂�
		if (character == L"�@") { continue; }

		//�u���v�����o���ꂽ����s
		if (character == L"��") {
			posCounter.x = 0;
			posCounter.y++;
			continue;
		}

		//�����X�v���C�g����
		DirectX::XMFLOAT2 pos = { leftTopPos.x + (CharSprite::charTextureSize - 2) * scale * posCounter.x, leftTopPos.y + (CharSprite::charTextureSize + 2) * posCounter.y };
		charSprites.push_back(std::make_unique<CharSprite>(character, pos, scale, isStartDraw));

		//���̕��������ɂ��炷
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
