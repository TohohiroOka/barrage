#include "TextManager.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "WindowApp.h"
#include "Math/Easing/Easing.h"

TextManager::TextManager()
{
	//�g�X�v���C�g�𐶐�
	const DirectX::XMFLOAT2 framePos = { (float)WindowApp::GetWindowWidth() / 2, 700 };
	sentenceFrameSprite = Sprite::Create("white", framePos, { 0.5f, 0.5f }, { 0.1f, 0.1f, 0.1f, 0 });
	DirectX::XMFLOAT2 backSize = { 900, 220 };
	sentenceFrameSprite->SetSize(backSize);

	//���̃e�L�X�g�𐶐�������̓X�v���C�g�𐶐�
	std::string textureName;
	if (XInputManager::GetInstance()->ControllerConnectCheck()) { textureName = "pad_" + std::to_string(XInputManager::PAD_A); }
	else { textureName = "key_" + std::to_string(DIK_SPACE); }
	inputNextTextSprite = Sprite::Create(textureName, { 1150, 775 }, { 0.5, 0.5f });
	inputNextTextSprite->SetScale(1.0f);

	//�e��Ԃ̓��e���Z�b�g
	func.emplace_back([this] { return UpdateSentenceNonePhase(); });
	func.emplace_back([this] { return UpdateSentenceFrameEmergePhase(); });
	func.emplace_back([this] { return UpdateSentenceCreatePhase(); });
	func.emplace_back([this] { return UpdateSentenceFrameSubemergePhase(); });

	//�^�C�}�[�N���X
	easeTimer = std::make_unique<Engine::Timer>();
}

TextManager::~TextManager()
{
}

void TextManager::Update()
{
	//�g�X�v���C�g�X�V
	if (!(sentenceCreatePhase == SentenceCreatePhase::NONE)) {
		sentenceFrameSprite->Update();
	}
	//���̓X�v���C�g�`��
	if (sentence.text && !choices.question) {
		if (sentence.text->GetIsAllWrite()) {
			std::string textureName;
			if (XInputManager::GetInstance()->ControllerConnectCheck()) { textureName = "pad_" + std::to_string(XInputManager::PAD_A); }
			else { textureName = "key_" + std::to_string(DIK_SPACE); }
			inputNextTextSprite->SetTexture(textureName);
			inputNextTextSprite->SetScale(1.f);
			inputNextTextSprite->Update();
		}
	}

	//���͂̊e��Ԃ̓��e���X�V
	func[int(sentenceCreatePhase)]();

	//�I�������Z�b�g����Ă���Ȃ�I�����X�V
	if (choices.question) {
		choices.question->Update();
	}
}

void TextManager::Draw()
{
	//�g�X�v���C�g�`��
	if (!(sentenceCreatePhase == SentenceCreatePhase::NONE)) {
		sentenceFrameSprite->Draw();
	}

	//���͐�����ԂȂ�e�L�X�g�`��
	if (sentenceCreatePhase == SentenceCreatePhase::SENTENCE_CREATE) {
		sentence.text->Draw();
	}

	//�e�L�X�g�̕\�����I�����Ă���Γ��̓X�v���C�g�`��
	if (sentence.text && !choices.question) {
		if (sentence.text->GetIsAllWrite()) {
			inputNextTextSprite->Draw();
		}
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

	//���͐�����Ԃ��Ȃɂ��Ȃ��t�F�[�Y�Ȃ�g���o�Ă���t�F�[�Y�ɍX�V
	if (sentenceCreatePhase == SentenceCreatePhase::NONE) {
		sentenceCreatePhase = SentenceCreatePhase::FRAME_EMERGE;
	}
}

void TextManager::SentenceDrawEnd()
{
	//�g�X�v���C�g�����ł�����
	sentenceCreatePhase = SentenceCreatePhase::FRAME_SUBMERGE;

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

void TextManager::Finalize()
{
	sentenceFrameSprite.reset();
	inputNextTextSprite.reset();
}

void TextManager::UpdateSentenceNonePhase()
{
	//�Ȃɂ����Ȃ��t�F�[�Y
}

void TextManager::UpdateSentenceFrameEmergePhase()
{
	//�g�X�v���C�g�o���ɂ����鎞��
	const float emergeTime = 10;
	easeTimer->Update();
	//�C�[�W���O�p�^�C���������Z�o
	const float time = *easeTimer.get() / emergeTime;

	//�F�̓����x���C�[�W���O�ŕύX
	DirectX::XMFLOAT4 color = sentenceFrameSprite->GetColor();
	color.w = Easing::Lerp(0, frameColorAlpha, time);
	sentenceFrameSprite->SetColor(color);

	//�^�C�}�[���w�肵�����ԂɂȂ����當�͂𐶐������ԂɕύX
	if (*easeTimer.get() >= emergeTime) {
		sentenceCreatePhase = SentenceCreatePhase::SENTENCE_CREATE;

		//�^�C�}�[�����Z�b�g���Ă���
		easeTimer->Reset();
	}
}

void TextManager::UpdateSentenceCreatePhase()
{
	//���̃e�L�X�g������Ȃ�X�V
	SentenceNextText();

	//�X�v���C�g�X�V
	sentence.text->Update();
}

void TextManager::UpdateSentenceFrameSubemergePhase()
{
	//�g�X�v���C�g���łɂ����鎞��
	const float submergeTime = 10;
	easeTimer->Update();
	//�C�[�W���O�p�^�C���������Z�o
	const float time = *easeTimer.get() / submergeTime;

	//�F�̓����x���C�[�W���O�ŕύX
	DirectX::XMFLOAT4 color = sentenceFrameSprite->GetColor();
	color.w = Easing::Lerp(frameColorAlpha, 0, time);
	sentenceFrameSprite->SetColor(color);

	//�^�C�}�[���w�肵�����ԂɂȂ����當�͂��Ȃɂ����Ȃ���ԂɕύX
	if (*easeTimer.get() >= submergeTime) {
		sentenceCreatePhase = SentenceCreatePhase::NONE;

		//�^�C�}�[�����Z�b�g���Ă���
		easeTimer->Reset();
	}
}

void TextManager::SentenceNextText()
{
	//�e�L�X�g�̕\�����I�����Ă��Ȃ���Δ�����
	if (!sentence.text->GetIsAllWrite()) { return; }
	//���͂��Ȃ���Δ�����
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_SPACE) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A))) { return; }

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
