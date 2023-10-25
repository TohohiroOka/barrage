#include "TitleScene.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Scene/SceneManager.h"
#include "scene/Scene1.h"

void TitleScene::Initialize()
{
	//�X�v���C�g�̃��\�[�X�̃��[�h
	Sprite::LoadTexture("titleLogo", "Resources/SpriteTexture/titleLogo.png");
	//�X�v���C�g����
	titleLogoSprite = Sprite::Create("titleLogo", {}, { 640.f / 1.0f,64.f / 1.0f });
	titleLogoSprite->SetTexSize({ 640.f,64.0f });
	titleLogoSprite->SetAnchorpoint({ 0.5f, 0.5f });
	titleLogoSprite->SetPosition({ 1500.f / 2.f,200.f });
	titleLogoSprite->Update();

	//�J����������
	debugCamera = DebugCamera::Create({ 300, 40, 0 });
	lightCamera.reset(new LightCamera({ -50, 20, -50 }));
	lightCamera->SetProjectionNum({ 360, 300 }, { -360, -100 });
	//�J�����ݒ�
	Base3D::SetCamera(debugCamera.get());
	Base3D::SetLightCamera(lightCamera.get());
	//�n�`����
	field = std::make_unique<Field>();


}

void TitleScene::Update()
{
	//�C���X�^���X�擾
	DirectInput* dinput = DirectInput::GetInstance();
	XInputManager* xinput = XInputManager::GetInstance();
	//�n�`�X�V
	field->Update();
	//�J�����X�V
	debugCamera->Update();
	lightCamera->Update();

	//�V�[���؂�ւ�
	if (dinput->ReleaseKey(DIK_2) || 
		dinput->ReleaseKey(DIK_SPACE) || 
		xinput->TriggerButton(xinput->PAD_B)) {
		Scene1* gameScene = new Scene1;
		SceneManager::SetNextScene(gameScene);
	}
}

void TitleScene::Draw(const int _cameraNum)
{
	field->Draw();
}

void TitleScene::DrawLightView(const int _cameraNum)
{
}

void TitleScene::NonPostEffectDraw(const int _cameraNum)
{
	titleLogoSprite->Draw();
}

void TitleScene::Finalize()
{
}

void TitleScene::ImguiDraw()
{
}

void TitleScene::FrameReset()
{
}

void TitleScene::CollisionCheck()
{
}
