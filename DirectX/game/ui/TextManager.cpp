#include "TextManager.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"

TextManager::TextManager()
{
}

TextManager::~TextManager()
{
}

void TextManager::Update()
{
	//�e�L�X�g���Z�b�g����Ă���Ȃ�e�L�X�g�X�V
	if (sentence.text) {
		SentenceNextText();
		sentence.text->Update();
	}

	//�I�������Z�b�g����Ă���Ȃ�I�����X�V
	if (choices.question) {
		choices.question->Update();
	}
}

void TextManager::Draw()
{
	//�e�L�X�g���Z�b�g����Ă���Ȃ�e�L�X�g�`��
	if (sentence.text) {
		sentence.text->Draw();
	}

	//�I�������Z�b�g����Ă���Ȃ�I�����X�V
	if (choices.question) {
		choices.question->Draw();
	}
}

void TextManager::SentenceCreate(SentenceData::SentenceName sentenceName)
{
	//���ɕ��̓e�L�X�g���\������Ă����甲����
	if (sentence.text) { return; }

	//�e�L�X�g�\���񐔂����Z�b�g
	textCount = 0;
	isSentenceEnd = false;

	//���͂ɕ��͖����Z�b�g
	sentence.sentenceName = sentenceName;
	//�Z�b�g���ꂽ���͂̍ŏ��̃e�L�X�g��ǂݍ���
	sentence.text = std::make_unique<TextTypeWriter>(TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]], textPos, textScale, textWriteSpeed);

	//�e�L�X�g�\���񐔂��X�V
	textCount++;
}

void TextManager::SentenceDrawEnd()
{
	//�e�L�X�g���������
	sentence.text.reset();

	//�e�L�X�g�\���񐔂����Z�b�g
	textCount = 0;
	isSentenceEnd = false;
}

void TextManager::ChoicesCreate(ChoicesData::ChoicesName choicesName)
{
	//���ɑI�������\������Ă����甲����
	if (choices.question) { return; }

	//���͂ɕ��͖����Z�b�g
	choices.choicesName = choicesName;
	//�Z�b�g���ꂽ�I�����V�X�e����ǂݍ���
	choices.question = std::make_unique<QuestionSystem>(ChoicesData::choicesData[(int)choices.choicesName]);
}

void TextManager::ChoicesDrawEnd()
{
	//�e�L�X�g���������
	choices.question.reset();
}

void TextManager::SentenceNextText()
{
	//�e�L�X�g�̕\�����I�����Ă��Ȃ���Δ�����
	if (!sentence.text->GetIsAllWrite()) { return; }
	//���͂��Ȃ���Δ�����
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_E) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A))) { return; }

	//���͂̃e�L�X�g���ɕ\���񐔂��B���Ă�����I���t���O�𗧂ĂĔ�����
	if (textCount >= (int)SentenceData::sentenceData[(int)sentence.sentenceName].size()) {
		isSentenceEnd = true;
		return;
	}

	//�Z�b�g����Ă��镶�͂̎��̃e�L�X�g��ǂݍ���
	sentence.text = std::make_unique<TextTypeWriter>(TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]], textPos, textScale, textWriteSpeed);

	//�e�L�X�g�\���񐔂��X�V
	textCount++;
}
