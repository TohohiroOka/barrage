#pragma once
#include "TextTypeWriter.h"
#include "QuestionSystem.h"
#include "TextData.h"
#include "Singleton.h"

/// <summary>
/// �e�L�X�g�Ǘ�
/// </summary>
class TextManager : public Singleton<TextManager>
{
	friend class Singleton<TextManager>;

public: //�\����
	/// <summary>
	/// ����
	/// </summary>
	struct Sentence
	{
		std::unique_ptr<TextTypeWriter> text; //�e�L�X�g
		SentenceData::SentenceName sentenceName; //���͖�
	};

	/// <summary>
	/// �I����
	/// </summary>
	struct Choices
	{
		std::unique_ptr<QuestionSystem> question; //�I�����V�X�e��
		ChoicesData::ChoicesName choicesName; //�I������
	};

public: //�����o�֐�
	TextManager();
	~TextManager();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���͐���
	/// </summary>
	/// <param name="sentenceName">���͖�</param>
	void SentenceCreate(SentenceData::SentenceName sentenceName);

	/// <summary>
	/// ���͂̕\���I��
	/// </summary>
	void SentenceDrawEnd();


	/// <summary>
	/// �I��������
	/// </summary>
	/// <param name="choicesName">�I������</param>
	void ChoicesCreate(ChoicesData::ChoicesName choicesName);

	/// <summary>
	/// �I�����̕\���I��
	/// </summary>
	void ChoicesDrawEnd();

	//getter
	bool GetIsTextDraw() { return sentence.text || choices.question; }
	const Sentence& GetSentece() { return sentence; }
	bool GetIsSentenceEnd(SentenceData::SentenceName sentenceName) {
		//���͖�������Ă�����false
		if (!(sentence.sentenceName == sentenceName)) { return false; }

		return isSentenceEnd;
	}
	const Choices& GetChoices() { return choices; }
	bool GetIsChoiceEnd() {
		//���������I�������\������Ă��Ȃ����false
		if (!choices.question) { return false; }
		return choices.question->GetIsEnd(); 
	}
	int GetSelectNum(ChoicesData::ChoicesName choicesName) {
		//�I������������Ă�����ǂł���������Ԃ�
		if (!(choices.choicesName == choicesName)) { return 1000; }

		return choices.question->GetSelectNum();
	}

private: //�����o�֐�
	/// <summary>
	/// ���̃e�L�X�g��\��
	/// </summary>
	void SentenceNextText();

private: //�萔
	//�e�L�X�g1�����ڂ̍��W
	const DirectX::XMFLOAT2 textPos = { 400, 640 };
	//�e�L�X�g�̑傫��
	const float textScale = 1.0f;
	//�e�L�X�g��\�����鑁��
	const int textWriteSpeed = 2;

private: //�����o�ϐ�
	//����
	Sentence sentence;
	//1���͂̃e�L�X�g�X�V��
	int textCount = 0;
	//���͂�\�����I������
	bool isSentenceEnd = false;

	//�I����
	Choices choices;
};