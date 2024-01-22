#include "NumberText.h"

NumberText::NumberText()
{
}

NumberText::~NumberText()
{
}

void NumberText::InsertCharSprite(CharSprite* charSprite)
{
	//vector�z��ɒǉ�
	charSprites.push_back(charSprite);
}

void NumberText::InsertEnd()
{
	//�ŏI�I�ɑS�Ă̗v�f�̐������q����
	std::wstring num;
	for (auto& charSprite : charSprites) {
		num.push_back(*charSprite->GetCharacter().c_str());
	}

	//�q����������ۑ�����
	number = std::stoi(num);
}

void NumberText::ChangeNumber(int number)
{
	//���ɓ����Ă��鐔���Ɠ��������Ȃ甲����
	if (this->number == number) { return; }

	//������ύX
	this->number = number;

	//�e�L�X�g�̐������ύX
	int roopNum = 0;
	for (auto& charSprite : charSprites) {
		//���ꂼ��̌��̐������Z�o���ĕ����ɕύX����
		int digitDiv = 1; //����������l
		for (int i = 0; i < (int)charSprites.size() - roopNum; i++) {
			if (i == 0) { continue; }
			digitDiv *= 10;
		}
		const int digitNum = (number / digitDiv) % 10;
		charSprite->ChangeCharacter(std::to_wstring(digitNum));

		//���[�v�񐔂𑝂₷
		roopNum++;
	}
}
