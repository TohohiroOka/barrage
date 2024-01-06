#include "TitleScene.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Scene/SceneManager.h"
#include "scene/Scene1.h"
#include "cutscene/SceneChangeDirection.h"
#include "Audio/Audio.h"
#include "WindowApp.h"
#include <Object/3d/collider/Collision.h>

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	//�X�v���C�g�̃��\�[�X�̃��[�h
	TextureManager::LoadTexture("titleLogo", "Resources/SpriteTexture/titleLogo.png");
	//�X�v���C�g����
	titleLogoSprite = Sprite::Create("titleLogo", {}, { 0.5f,0.5f });
	titleLogoSprite->SetSize({ 252.f * 1.5f,59.0f * 1.5f });
	titleLogoSprite->SetPosition({ 1500.f / 2.f,100.f });
	titleLogoSprite->Update();

	pressSelectButtonUI = std::make_unique<PressSelectButtonUI>();

	//Audio::Instance()->SoundPlayWava(Sound::SoundName::msp_bgm, true, 0.1f);

	//�n�`����
	field = std::make_unique<Field>();
	//�v���C���[����
	player = std::make_unique<Player>();
	//�|�[�^������
	Scene1* gameScene = new Scene1;
	const float stageSize = GameHelper::Instance()->GetStageSize();
	const float portalPosY = 11.0f;
	portals[0] = std::make_unique<Portal>(Vector3{ stageSize * 0.75f, portalPosY, stageSize / 2 }, gameScene);
	TitleScene* titleScene = new TitleScene;
	portals[1] = std::make_unique<Portal>(Vector3{ stageSize * 0.25f, portalPosY, stageSize / 2 }, titleScene);
	portals[2] = std::make_unique<Portal>(Vector3{ stageSize / 2, portalPosY, stageSize * 0.75f }, nullptr);

	//�J��������
	GameCamera::SetPlayer(player.get());
	debugCamera = DebugCamera::Create({ 300, 40, 0 });
	camera = std::make_unique<TitleCamera>();
	player->SetGameCamera(camera.get());

	//�e�p�����J����������
	lightCamera = std::make_unique<LightCamera>(Vector3{ 205, 200, 204 }, Vector3{ 205, 0, 205 });
	const float projectionSize = 1.5f;
	lightCamera->SetProjectionNum({ projectionSize * (float)WindowApp::GetWindowWidth() / 5, projectionSize * (float)WindowApp::GetWindowHeight() / 5 },
		{ -projectionSize * (float)WindowApp::GetWindowWidth() / 5, -projectionSize * (float)WindowApp::GetWindowHeight() / 5 });

	Base3D::SetCamera(camera.get());
	Base3D::SetLightCamera(lightCamera.get());

	ParticleManager::SetCamera(camera.get());

	//�s�����͐ݒ�
	actionInputConfig = std::make_unique<ActionInputConfig>();

	//�J�ڏ�����
	SceneChangeDirection::Instance()->Init();
}

void TitleScene::Update()
{
	if (!isInputConfigMode) {
		//�|�[�^���ɓ���s��
		IntoPortalStart();
		IntoPortal();
		//�Ó]�����������玟�̃V�[����
		if (isSceneChangeWait && SceneChangeDirection::Instance()->IsDirectionEnd()) {
			//�I�����Ă���|�[�^���̒ʂ���V�[���Ɉڍs����
			if (selectPortal->GetChangeScene() == nullptr) { isEndRequest = true; } //�Q�[�����[�v�I��
			else { SceneManager::SetNextScene(selectPortal->GetChangeScene()); } //�w�肵���V�[���ֈڍs
		}

		//�I�u�W�F�N�g�X�V
		player->Update();
		field->Update(player->GetData()->pos, camera->GetEye());
		for (int i = 0; i < 3; i++) {
			portals[i]->Update(*player->GetData());
		}

		//�����蔻��
		CollisionCheck();

		//�J�����X�V
		if (isNormalCamera) {
			camera->Update();
			if (DirectInput::GetInstance()->TriggerKey(DIK_RETURN)) {
				isNormalCamera = !isNormalCamera;
				Base3D::SetCamera(debugCamera.get());
			}
		}
		else {
			debugCamera->Update();
			if (DirectInput::GetInstance()->TriggerKey(DIK_RETURN)) {
				isNormalCamera = !isNormalCamera;
				Base3D::SetCamera(camera.get());
			}
		}
		lightCamera->Update();

		if ((!isIntoPortal) && (DirectInput::GetInstance()->TriggerKey(DIK_TAB) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_START))) {
			isInputConfigMode = true;
			actionInputConfig->Reset();
		}
	}
	else {
		//���͐ݒ�X�V
		actionInputConfig->Update();

		if (actionInputConfig->GetIsInputConfigEnd()) { isInputConfigMode = false; }
	}

	//�X�v���C�g�X�V
	pressSelectButtonUI->Update();

	SceneChangeDirection::Instance()->Update();
}

void TitleScene::Draw(const int _cameraNum)
{
	player->Draw();
	field->Draw();
	for (int i = 0; i < 3; i++) {
		portals[i]->Draw();
	}
}

void TitleScene::DrawLightView(const int _cameraNum)
{
	player->DrawLightView();
	for (int i = 0; i < 3; i++) {
		portals[i]->DrawLightView();
	}
}

void TitleScene::NonPostEffectDraw(const int _cameraNum)
{
	titleLogoSprite->Draw();

	bool isIntoPortal = false;
	for (int i = 0; i < 3; i++) {
		if (portals[i]->GetIsIntoPortal()) {
			isIntoPortal = true;
			break;
		}
	}
	if (isIntoPortal) {
		pressSelectButtonUI->Draw();
	}

	//���͐ݒ�`��
	if (isInputConfigMode) {
		actionInputConfig->Draw();
	}


	SceneChangeDirection::Instance()->Draw();
}

void TitleScene::ImguiDraw()
{
}

void TitleScene::FrameReset()
{
	player->FrameReset();
	field->FrameReset();
}

void TitleScene::CollisionCheck()
{
#pragma region �v���C���[�ƃ|�[�^���̏Փ˔���
	{
		const Vector3 plpos = player->GetData()->pos;
		Sphere playerSphere;
		playerSphere.center = { plpos.x, plpos.y, plpos.z, 1.0f };
		playerSphere.radius = player->GetFbxObject()->GetScale().x / 2;

		for (int i = 0; i < 3; i++) {
			const Vector3 popos = portals[i]->GetObject3d()->GetPosition();
			Sphere portalSphere;
			portalSphere.center = { popos.x, popos.y, popos.z, 1.0f };
			portalSphere.radius = portals[i]->GetObject3d()->GetScale().x;

			XMVECTOR inter;
			XMVECTOR reject;
			if (Collision::CheckSphere2Sphere(playerSphere, portalSphere, &inter, &reject)) {
				//�v���C���[�������߂�
				player->PushBack(reject);
			}
		}
	}
#pragma endregion
}

void TitleScene::IntoPortalStart()
{
	//���Ƀ|�[�^���ɓ���s�������Ă���Δ�����
	if (isIntoPortal) { return; }
	//���͂��Ȃ���Δ�����
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_E) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A))) { return; }
	//�J�������ʏ��ԂłȂ���Δ�����
	if (!(camera->GetTitleCameraPhase() == TitleCamera::TitleCameraPhase::NORMAL)) { return; }

	//�|�[�^���̐���
	for (int i = 0; i < 3; i++) {
		if (!portals[i]->GetIsIntoPortal()) { continue; }

		camera->SetPortalPos(portals[i]->GetObject3d()->GetPosition());
		camera->ChangePhase(TitleCamera::TitleCameraPhase::MOVE_PORTAL_FRONT);
		player->SetPortalPos(portals[i]->GetObject3d()->GetPosition());
		player->TitlePhaseStart();
		isIntoPortal = true;
		selectPortal = portals[i].get();
		break;
	}
}

void TitleScene::IntoPortal()
{
	//�|�[�^���ɓ���s�������Ă��Ȃ���Δ�����
	if (!isIntoPortal) { return; }

	//�J�������|�[�^�����ʂɈړ������Ԃ̂Ƃ�
	if (camera->GetTitleCameraPhase() == TitleCamera::TitleCameraPhase::MOVE_PORTAL_FRONT) {
		//�v���C���[���|�[�^���ɓ���s����ԂȂ�΁A�J�����s�����|�[�^���ɃY�[�������ԂɕύX
		if (!(player->GetData()->action == PlayerActionName::TITLE_INTO_PORTAL)) { return; }
		camera->ChangePhase(TitleCamera::TitleCameraPhase::ZOOM_PORTAL);
	}
	//�J�������|�[�^�����Y�[�������Ԃ̂Ƃ�
	else if (camera->GetTitleCameraPhase() == TitleCamera::TitleCameraPhase::ZOOM_PORTAL) {
		//�J�����s�����|�[�^���ɃY�[������s�����I������A��~���
		if (!camera->GetIsPhaseActionEnd()) { return; }
		camera->ChangePhase(TitleCamera::TitleCameraPhase::STAY);
	}
	//�J�������|�[�^���O�Œ�~��Ԃ̂Ƃ�
	else if (camera->GetTitleCameraPhase() == TitleCamera::TitleCameraPhase::STAY) {
		//��~��Ԃ��I������A�Ó]�J�n
		if (!camera->GetIsPhaseActionEnd()) { return; }
		if (isSceneChangeWait) { return; }

		isSceneChangeWait = true;
		SceneChangeDirection::Instance()->PlayFadeOut();
	}
}
