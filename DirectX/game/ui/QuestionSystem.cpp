#include "QuestionSystem.h"
#include "WindowApp.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "Math/Easing/Easing.h"

QuestionSystem::QuestionSystem(const std::vector<std::wstring>& choices)
{
	//�I���������Z�b�g
	choicesNum = (int)choices.size();

	//�I�����̃e�L�X�g�X�v���C�g�𐶐�
	centerPos = { (float)WindowApp::GetWindowWidth() / 2, (float)WindowApp::GetWindowHeight() / 2 - (float)WindowApp::GetWindowHeight() * 0.05f };
	const float textScale = 1.2f;
	int charNumMax = 0;
	for (int i = 0; i < choicesNum; i++) {
		std::unique_ptr<TextCreator> newText;
		const DirectX::XMFLOAT2 pos = {
			centerPos.x - ((CharSprite::charTextureSize - 2) * textScale) / 2 * (float)(choices[i].size() - 1),
			(centerPos.y - ((CharSprite::charTextureSize + 15) * textScale / 2 * (float)(choices.size() - 1)) + (CharSprite::charTextureSize + 15) * textScale * i)
		};
		newText = std::make_unique<TextCreator>(choices[i], pos, textScale);
		choiceTextSprites.push_back(std::move(newText));

		//Y���W���L�����Ă���
		textPosYs.push_back(pos.y);
		//�ő啶�����`�F�b�N
		if (charNumMax < (int)choices[i].size()) {
			charNumMax = (int)choices[i].size();
		}
	}

	//�w�i�X�v���C�g����
	backSprite = Sprite::Create("white", centerPos, { 0.5f, 0.5f }, { 0.3f, 0.3f, 0.3f, 0.8f });
	DirectX::XMFLOAT2 backSize = { (CharSprite::charTextureSize - 2) * textScale * charNumMax + 80, (CharSprite::charTextureSize + 15) * (float)(choices.size()) * textScale + 30 };
	//�g������������Ƃ���ڂ��̂ŁA�ŏ��l��ݒ�
	backSize.x = max(backSize.x, 350);
	backSize.y = max(backSize.y, 150);
	backSprite->SetSize(backSize);

	//�I�������w�i�p�X�v���C�g����
	choiceBackSprite = Sprite::Create("white", DirectX::XMFLOAT2{ centerPos.x, textPosYs[selectNum] }, { 0.5f, 0.5f }, { 1, 0, 0, 0.8f });
	DirectX::XMFLOAT2 choiceBackSize = { backSize.x - 60, CharSprite::charTextureSize * textScale + 4 };
	choiceBackSprite->SetSize(choiceBackSize);

	//�e��Ԃ̓��e���Z�b�g
	func.emplace_back([this] { return StartWaitUpdate(); });
	func.emplace_back([this] { return SelectChoiceUpdate(); });
	func.emplace_back([this] { return ConfirmSelectionUpdate(); });

	//�^�C�}�[�N���X
	timer = std::make_unique<Engine::Timer>();
}

QuestionSystem::~QuestionSystem()
{
}

void QuestionSystem::Update()
{
	//�e��Ԃ̓��e���X�V
	func[int(phase)]();

	//�X�v���C�g�X�V
	backSprite->Update();
	choiceBackSprite->Update();
	for (auto& choiceTextSprite : choiceTextSprites) {
		choiceTextSprite->Update();
	}
}

void QuestionSystem::Draw()
{
	backSprite->Draw();
	//�ҋ@��Ԃ̂Ƃ��͑I�������w�i�X�v���C�g��`�悵�Ȃ�
	if (!(phase == QuestionPhase::START_WAIT)) {
		choiceBackSprite->Draw();
	}
	for (auto& choiceTextSprite : choiceTextSprites) {
		choiceTextSprite->Draw();
	}
}

void QuestionSystem::StartWaitUpdate()
{
	//�{�^���A�łɂ��I����h�����߂ɑҋ@�����鎞��
	const float waitTime = 30;
	//�^�C�}�[�X�V
	timer->Update();

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (*timer.get() >= waitTime) {
		phase = QuestionPhase::SELECT_CHOICE;

		//�^�C�}�[�����Z�b�g���Ă���
		timer->Reset();
	}
}

void QuestionSystem::SelectChoiceUpdate()
{
	//�I����ύX����
	ChangeSelectChoices();
	//�I�������肷��
	SelectChoice();
}

void QuestionSystem::ChangeSelectChoices()
{
	//���͂��Ȃ���Δ�����
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_UP) || DirectInput::GetInstance()->TriggerKey(DIK_DOWN) ||
		DirectInput::GetInstance()->TriggerKey(DIK_W) || DirectInput::GetInstance()->TriggerKey(DIK_S) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN) || 
		XInputManager::GetInstance()->TriggerLeftStickY(true) || XInputManager::GetInstance()->TriggerLeftStickY(false))) {
		return;
	}

	//����͂ň��̑I������I������
	if (DirectInput::GetInstance()->TriggerKey(DIK_UP) || DirectInput::GetInstance()->TriggerKey(DIK_W) || 
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP) || XInputManager::GetInstance()->TriggerLeftStickY(true)) {
		if (selectNum <= 0) { return; }
		selectNum--;
	}
	//�����͂ň���̑I������I������
	else if (DirectInput::GetInstance()->TriggerKey(DIK_DOWN) || DirectInput::GetInstance()->TriggerKey(DIK_S) || 
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN) || XInputManager::GetInstance()->TriggerLeftStickY(false)) {
		if (selectNum >= choicesNum - 1) { return; }
		selectNum++;
	}

	//�I�������w�i�X�v���C�g�̍��W���X�V
	choiceBackSprite->SetPosition({ centerPos.x, textPosYs[selectNum] });
}

void QuestionSystem::SelectChoice()
{
	//���͂��Ȃ���Δ�����
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_SPACE) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A))) { return; }

	//�I���m��t�F�[�Y�ւ̈ڍs
	phase = QuestionPhase::CONFIRM_SELECTION;
}

void QuestionSystem::ConfirmSelectionUpdate()
{
	//�m�肵�����o1��ɂ����鎞��
	const float selectDirectionTime = 10;
	//�^�C�}�[�X�V
	timer->Update();

	//�F�̓����x���C�[�W���O�ŕύX
	const float time = *timer.get() / selectDirectionTime;
	DirectX::XMFLOAT4 color = choiceBackSprite->GetColor();
	color.w = Easing::Lerp(0.8f, 0, time);
	choiceBackSprite->SetColor(color);

	//�^�C�}�[���w�肵�����ԂɂȂ�����J�E���g���X�V
	if (*timer.get() >= selectDirectionTime) {
		selectDirectionCount++;

		//�^�C�}�[�����Z�b�g���Ă���
		timer->Reset();
	}

	//�J�E���g���w�肵���񐔂ɓ��B�����玿��I��
	const int selectDirectionNum = 6;
	if (selectDirectionCount >= selectDirectionNum) {
		isEnd = true;
	}
}
