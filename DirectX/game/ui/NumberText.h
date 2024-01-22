#pragma once
#include "CharSprite.h"

/// <summary>
/// �ϓ����鐔���e�L�X�g
/// </summary>
class NumberText
{
public: //�����o�֐�
	NumberText();
	~NumberText();

	/// <summary>
	/// �����e�L�X�g�Ƃ��Ďg�p���镶���X�v���C�g��ǉ�
	/// </summary>
	/// <param name="charSprite">�����X�v���C�g</param>
	void InsertCharSprite(CharSprite* charSprite);

	/// <summary>
	/// �����X�v���C�g�ǉ����I������
	/// </summary>
	void InsertEnd();

	/// <summary>
	/// �����ύX
	/// </summary>
	/// <param name="number"></param>
	void ChangeNumber(int number);

	//getter
	int GetNumber() { return number; }


private: //�����o�ϐ�
	//�����e�L�X�g�Ƃ��Ďg�p���镶���X�v���C�g�Q
	std::vector<CharSprite*> charSprites;
	//�e�L�X�g�Ƃ��ĕ\�����鐔��
	int number;
};