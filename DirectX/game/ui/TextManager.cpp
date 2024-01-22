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
	inputNextTextSprite = std::make_unique<CharActionInputSprite>(GameInputManager::InputAction::Select, DirectX::XMFLOAT2{ 1150, 780 }, 1.0f, true);

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
	if (sentence.textCreator && !choices.question) {
		//�e�L�X�g���Ƃ́u���͂ɂ��e�L�X�g�X�V�t���O�v��true�Ȃ�
		if (isInputNextText) {
			if (sentence.textCreator->GetIsAllWrite()) {
				inputNextTextSprite->Update();
			}
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
		sentence.textCreator->Draw();
	}

	//�e�L�X�g�̕\�����I�����Ă���Γ��̓X�v���C�g�`��
	if (sentence.textCreator && !choices.question) {
		//�e�L�X�g���Ƃ́u���͂ɂ��e�L�X�g�X�V�t���O�v��true�Ȃ�
		if (isInputNextText) {
			if (sentence.textCreator->GetIsAllWrite()) {
				inputNextTextSprite->Draw();
			}
		} 
	}

	//�I�������Z�b�g����Ă���Ȃ�I�����X�V
	if (choices.question) {
		choices.question->Draw();
	}
}

void TextManager::SentenceCreate(SentenceData::SentenceName sentenceName)
{
	//�e�L�X�g�̕\�����I�����Ă����
	if (!isSentenceEnd && sentence.textCreator->GetIsAllWrite()) {
		//���͂̃e�L�X�g���ɕ\���񐔂��B���Ă�����I���t���O�𗧂Ă�
		if (textCount >= (int)SentenceData::sentenceData[(int)sentence.sentenceName].size()) {
			isSentenceEnd = true;
		}
	}

	//����ł����͕\�����I���Ă��Ȃ���Δ�����
	if (!isSentenceEnd) { return; }

	//�e�L�X�g�\���񐔂����Z�b�g
	textCount = 0;
	isSentenceEnd = false;

	//�\�����I�������͂��\�����ꂽ�܂܂Ȃ�A�V���ȕ��͂����邽�ߕ��͂̃e�L�X�g��������Ă���
	if (sentence.textCreator) {
		sentence.textCreator.reset();
	}

	//���͂ɕ��͖����Z�b�g
	sentence.sentenceName = sentenceName;
	//�Z�b�g���ꂽ���͂̍ŏ��̃e�L�X�g��ǂݍ���
	sentence.text = TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].text;
	//���͂ɂ��e�L�X�g�X�V���\�ɂ��邩
	isInputNextText = TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].isInputNextText;

	//�e�L�X�g���ƂɎw�肵���A�^�C�v���C�^�[�V�X�e�����g�p���邩����
	if (TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].isTypeWriter) {
		sentence.textCreator = std::make_unique<TextTypeWriter>(sentence.text, textPos, textScale, textWriteSpeed);
	}
	else {
		sentence.textCreator = std::make_unique<TextCreator>(sentence.text, textPos, textScale, true);
	}

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
	sentence.textCreator.reset();

	//�e�L�X�g�\���񐔂����Z�b�g
	textCount = 0;
	isSentenceEnd = true;
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
	sentence.textCreator->Update();
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
	//�e�L�X�g���Ƃ́u���͂ɂ��e�L�X�g�X�V�t���O�v��false�Ȃ甲����
	if (!isInputNextText) { return; }

	//�e�L�X�g�̕\�����I�����Ă��Ȃ���Δ�����
	if (!sentence.textCreator->GetIsAllWrite()) { return; }
	//���͂��Ȃ���Δ�����
	if (!GameInputManager::TriggerInputAction(GameInputManager::Select)) { return; }

	//���͂̃e�L�X�g���ɕ\���񐔂��B���Ă�����I���t���O�𗧂ĂĔ�����
	if (textCount >= (int)SentenceData::sentenceData[(int)sentence.sentenceName].size()) {
		isSentenceEnd = true;
		return;
	}

	//�e�L�X�g���X�V
	//�Z�b�g����Ă��镶�͂̎��̃e�L�X�g��ǂݍ���
	sentence.text = TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].text;
	//���͂ɂ��e�L�X�g�X�V���\�ɂ��邩
	isInputNextText = TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].isInputNextText;

	//�e�L�X�g���ƂɎw�肵���A�^�C�v���C�^�[�V�X�e�����g�p���邩����
	if (TextData::textData[(int)SentenceData::sentenceData[(int)sentence.sentenceName][textCount]].isTypeWriter) {
		sentence.textCreator = std::make_unique<TextTypeWriter>(sentence.text, textPos, textScale, textWriteSpeed);
	}
	else {
		sentence.textCreator = std::make_unique<TextCreator>(sentence.text, textPos, textScale, true);
	}

	//�e�L�X�g�\���񐔂��X�V
	textCount++;
}
