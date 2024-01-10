#pragma once
#include "TextCreator.h"
#include "Math/Timer.h"

/// <summary>
/// �^�C�v���C�^�[(1�������`�悷����)
/// </summary>
class TextTypeWriter
{
public: //�����o�֐�
	TextTypeWriter(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, int writeWaitFrame = 1);
	~TextTypeWriter();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	bool GetIsAllWrite() { return text->GetCharSprite(text->GetTextLength() - 1)->GetIsDraw(); }

private: //�����o�֐�
	/// <summary>
	/// �\���Ǘ�
	/// </summary>
	void Write();

	/// <summary>
	/// ��C�ɑS�����\��
	/// </summary>
	void AllWrite();

private: //�ÓI�����o�ϐ�
	//�u�A�v�̑ҋ@�t���[����
	static const int commaWaitFrame = 10;
	//�u�B�v�̑ҋ@�t���[����
	static const int periodWaitFrame = 20;

private: //�����o�ϐ�
	//����
	std::unique_ptr<TextCreator> text;
	//�\���p�p�^�C�}�[
	std::unique_ptr<Engine::Timer> timer;
	//1�����\���ɑҋ@����t���[����
	int writeWaitFrame = 0;
	//�ʏ핶���\���ɑҋ@����t���[����
	int normalWriteWaitFrame = 0;
	//�\���������J�E���g
	int writeCount = 0;
};