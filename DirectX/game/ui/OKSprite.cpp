#include "OKSprite.h"
#include "WindowApp.h"
#include "Math/Easing/Easing.h"

OKSprite::OKSprite()
{
	//�X�v���C�g�̃��\�[�X�̃��[�h
	TextureManager::LoadTexture("OK", "Resources/SpriteTexture/OK.png");
	//�X�v���C�g����
	okSprite = Sprite::Create("OK", { (float)WindowApp::GetWindowWidth() / 2, (float)WindowApp::GetWindowHeight() / 2 - 50 }, { 0.5f, 0.5f });

	//�^�C�}�[����
	scaleChangeTimer = std::make_unique<Engine::Timer>();
}

OKSprite::~OKSprite()
{
}

void OKSprite::Update()
{
	//�`�悵�Ȃ���ԂȂ甲����
	if (!isDraw) { return; }

	//�X�P�[���ύX���o
	ScaleChange();

	okSprite->Update();
}

void OKSprite::Draw()
{
	//�`�悵�Ȃ���ԂȂ甲����
	if (!isDraw) { return; }

	okSprite->Draw();
}

void OKSprite::DrawStart()
{
	//�^�C�}�[������
	scaleChangeTimer->Reset();

	//�X�P�[����2�{�ɃZ�b�g���Ă���
	spriteScale = 2.0f;

	//�t���O���Z�b�g
	isDraw = true;
	isScaleChange = true;
}

void OKSprite::ScaleChange()
{
	//�X�P�[���ύX���o���łȂ���Δ�����
	if (!isScaleChange) { return; }

	//�^�C�}�[�X�V
	const float scaleChangeTime = 15;
	scaleChangeTimer->Update();
	const float time = *scaleChangeTimer.get() / scaleChangeTime;

	//�C�[�W���O�ŃX�P�[���ύX
	spriteScale = Easing::OutBack(2.0f, 1.0f, time);
	okSprite->SetScale(spriteScale);

	//�^�C�}�[���w�肵�����ԂɂȂ�����X�P�[���ύX���o�I��
	if (*scaleChangeTimer.get() >= scaleChangeTime) {
		isScaleChange = false;
	}
}
