#pragma once
#include "TextCreator.h"
#include "Math/Timer.h"

/// <summary>
/// �^�C�v���C�^�[(1�������`�悷����)
/// </summary>
class TextTypeWriter : public TextCreator
{
public: //�����o�֐�
	TextTypeWriter(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, int writeWaitFrame = 1);
	~TextTypeWriter();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// �\���Ǘ�
	/// </summary>
	void Write();

	/// <summary>
	/// �J�^�J�^���Ǘ�
	/// </summary>
	void TextSound();

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
	//�\���p�p�^�C�}�[
	std::unique_ptr<Engine::Timer> timer;
	//1�����\���ɑҋ@����t���[����
	int writeWaitFrame = 0;
	//�ʏ핶���\���ɑҋ@����t���[����
	int normalWriteWaitFrame = 0;
	//�\���������J�E���g
	int writeCount = 0;
	//�e�L�X�g�X�V�Ɠ������͂��g�p���Ă���ꍇ�A�g���K�[���肪�ʂ��Ă��܂����͂��S�\������Ă��܂��̂Ő��������u�Ԃ��̃t���O��ݒu
	bool isCreateFrame = true;

	//�T�E���h�ԍ�
	int soundNumber = 0;
};