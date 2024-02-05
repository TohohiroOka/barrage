#include "TextTypeWriter.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Audio/Audio.h"
#include "GameHelper.h"

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
	//�J�^�J�^���Đ�
	TextSound();

	//�\�������������u�A�v��u�B�v�Ȃǂ̏ꍇ�̓t���[������ύX
	if (charSprites[writeCount]->GetCharacter() == L"�A") {
		writeWaitFrame = commaWaitFrame;

		//�J�^�J�^���̎�ނ������̑傫�����ɖ߂�
		soundNumber = 0;
	}
	else if (charSprites[writeCount]->GetCharacter() == L"�B") {
		writeWaitFrame = periodWaitFrame;

		//�J�^�J�^���̎�ނ������̑傫�����ɖ߂�
		soundNumber = 0;
	}
	else {
		writeWaitFrame = normalWriteWaitFrame;
	}

	//�^�C�}�[���Z�b�g
	timer->Reset();

	//���̕����ԍ��ɍX�V
	writeCount++;
}

void TextTypeWriter::TextSound()
{
	//�J�^�J�^���Đ�
	if (soundNumber == 0) { Audio::Instance()->SoundPlayWava(Sound::SoundName::text_1, false, 0.1f); }
	else if (soundNumber == 1) { Audio::Instance()->SoundPlayWava(Sound::SoundName::text_2, false, 0.1f); }
	else if (soundNumber == 2) { Audio::Instance()->SoundPlayWava(Sound::SoundName::text_3, false, 0.1f); }

	//���ɖ炷����ݒ�
	int randamNum = RandomInt(1);
	if (randamNum == 0) {
		if (soundNumber == 0) { soundNumber = 1; }
		else if (soundNumber == 1) { soundNumber = 2; }
		else if (soundNumber == 2) { soundNumber = 0; }
	}
	else if (randamNum == 1) {
		if (soundNumber == 0) { soundNumber = 2; }
		else if (soundNumber == 1) { soundNumber = 0; }
		else if (soundNumber == 2) { soundNumber = 1; }
	}
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

	//�e�L�X�g�S�\�����Đ�
	Audio::Instance()->SoundPlayWava(Sound::SoundName::text_all_draw, false, 0.1f);
}
