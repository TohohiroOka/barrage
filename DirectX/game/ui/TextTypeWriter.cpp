#include "TextTypeWriter.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"

TextTypeWriter::TextTypeWriter(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, int writeWaitFrame)
	:TextCreator(text, leftTopPos, scale, false)
{
	//�ʏ�\���ҋ@�t���[�������Z�b�g
	this->normalWriteWaitFrame = writeWaitFrame;
	//�\���ҋ@�t���[�������Z�b�g
	this->writeWaitFrame = normalWriteWaitFrame;

	//�^�C�}�[����
	timer = std::make_unique<Engine::Timer>();
}

TextTypeWriter::~TextTypeWriter()
{
}

void TextTypeWriter::Update()
{
	//�\���Ǘ�
	Write();
	AllWrite();

	//�e�L�X�g�X�V
	TextCreator::Update();

	//�������ꂽ�u�Ԃ��I��
	if (isCreateFrame) {
		isCreateFrame = false;
	}
}

void TextTypeWriter::Write()
{
	//�S�Ă̕\�����I�����Ă���Δ�����
	if (GetIsAllWrite()) { return; }

	//�\���p�^�C�}�[�X�V
	timer->Update();
	//�ҋ@�t���[�����ɒB���Ă��Ȃ���Δ�����
	if (*timer.get() < writeWaitFrame) { return; }

	//������\����ԂɃZ�b�g
	charSprites[writeCount]->SetIsDraw(true);

	//�\�������������u�A�v��u�B�v�Ȃǂ̏ꍇ�̓t���[������ύX
	if (charSprites[writeCount]->GetCharacter() == L"�A") {
		writeWaitFrame = commaWaitFrame;
	}
	else if (charSprites[writeCount]->GetCharacter() == L"�B") {
		writeWaitFrame = periodWaitFrame;
	}
	else {
		writeWaitFrame = normalWriteWaitFrame;
	}

	//�^�C�}�[���Z�b�g
	timer->Reset();

	//���̕����ԍ��ɍX�V
	writeCount++;
}

void TextTypeWriter::AllWrite()
{
	//�S�Ă̕\�����I�����Ă���Δ�����
	if (GetIsAllWrite()) { return; }
	//���͂��Ȃ���Δ�����
	if (!GameInputManager::TriggerInputAction(GameInputManager::Select)) { return; }
	//�������ꂽ�u�ԂȂ甲����
	if (isCreateFrame) { return; }

	//�\������Ă��Ȃ���������C�ɑS�\������
	for (int i = writeCount; i < GetTextLength(); i++) {
		charSprites[i]->SetIsDraw(true);
	}
}
