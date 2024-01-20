#include "NumberText.h"

NumberText::NumberText()
{
}

NumberText::~NumberText()
{
}

void NumberText::InsertCharSprite(CharSprite* charSprite)
{
	//vector配列に追加
	charSprites.push_back(charSprite);
}

void NumberText::InsertEnd()
{
	//最終的に全ての要素の数字を繋げる
	std::wstring num;
	for (auto& charSprite : charSprites) {
		num.push_back(*charSprite->GetCharacter().c_str());
	}

	//繋げた数字を保存する
	number = std::stoi(num);
}

void NumberText::ChangeNumber(int number)
{
	//既に入っている数字と同じ数字なら抜ける
	if (this->number == number) { return; }

	//数字を変更
	this->number = number;

	//テキストの数字も変更
	int roopNum = 0;
	for (auto& charSprite : charSprites) {
		//それぞれの桁の数字を算出して文字に変更する
		int digitDiv = 1; //数字を割る値
		for (int i = 0; i < (int)charSprites.size() - roopNum; i++) {
			if (i == 0) { continue; }
			digitDiv *= 10;
		}
		const int digitNum = (number / digitDiv) % 10;
		charSprite->ChangeCharacter(std::to_wstring(digitNum));

		//ループ回数を増やす
		roopNum++;
	}
}
