#include "TutorialPause.h"
#include "WindowApp.h"
#include "ui/TextManager.h"

TutorialPause::TutorialPause()
{
	//���u���w�i�X�v���C�g������
	backSprite = Sprite::Create("white1x1", { 0,0 }, {}, { 0,0,0,0.7f });
	backSprite->SetSize({ (float)WindowApp::GetWindowWidth(), (float)WindowApp::GetWindowHeight() });
}

TutorialPause::~TutorialPause()
{
}

void TutorialPause::Update()
{
	//�|�[�Y��ʒ��łȂ���Δ�����
	if (!isPauseDraw) { return; }

	backSprite->Update();
}

void TutorialPause::Draw()
{
	//�|�[�Y��ʒ��łȂ���Δ�����
	if (!isPauseDraw) { return; }

	backSprite->Draw();
}

void TutorialPause::PauseStart()
{
	//���łɃ|�[�Y��ʒ��ł���Δ�����
	if (isPauseDraw) { return; }

	//�|�[�Y��ʒ��ɐݒ�
	isPauseDraw = true;

	//�`���[�g���A����p�|�[�Y��ʂ̃e�L�X�g�ƑI������\��
	TextManager::Instance()->ChoicesCreate(ChoicesData::ChoicesName::TUTORIAL_PAUSE_CHOICE);
}

void TutorialPause::PauseEnd()
{
	//�|�[�Y��ʏI��
	isPauseDraw = false;
	//�I�����̕\���I��
	TextManager::Instance()->ChoicesDrawEnd();
}
