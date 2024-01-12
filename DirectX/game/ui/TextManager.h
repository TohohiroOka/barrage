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

public: //enum
	/// <summary>
	/// ���͐����̃t�F�[�Y
	/// </summary>
	enum class SentenceCreatePhase
	{
		NONE,			//���͂��Ȃɂ��Ȃ��t�F�[�Y
		FRAME_EMERGE,	//���͂��͂��g���o�Ă���t�F�[�Y
		SENTENCE_CREATE,//���͂𐶐�����t�F�[�Y
		FRAME_SUBMERGE,	//���͂��͂��g�������Ă����t�F�[�Y
	};

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
	//�p���̓t�F�[�Y�̍X�V����
	void UpdateSentenceNonePhase();
	void UpdateSentenceFrameEmergePhase();
	void UpdateSentenceCreatePhase();
	void UpdateSentenceFrameSubemergePhase();


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
	//�g�X�v���C�g��alpha
	const float frameColorAlpha = 0.7f;

private: //�����o�ϐ�
	//���͐����̃t�F�[�Y
	SentenceCreatePhase sentenceCreatePhase = SentenceCreatePhase::NONE;
	//�e�t�F�[�Y�̓���
	std::vector<std::function<void()>> func;
	//�C�[�W���O�p�^�C�}�[
	std::unique_ptr<Engine::Timer> easeTimer;

	//���͂��͂��g�X�v���C�g
	std::unique_ptr<Sprite> sentenceFrameSprite;

	//���̃e�L�X�g��\�������邽�߂̃{�^���X�v���C�g
	std::unique_ptr<Sprite> inputNextTextSprite;

	//����
	Sentence sentence;
	//1���͂̃e�L�X�g�X�V��
	int textCount = 0;
	//���͂�\�����I������
	bool isSentenceEnd = false;

	//�I����
	Choices choices;
};