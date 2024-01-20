#include "TutorialScene.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Scene/SceneManager.h"
#include "cutscene/SceneChangeDirection.h"
#include "Audio/Audio.h"
#include "WindowApp.h"
#include "Object/3d/collider/Collision.h"
#include "TitleScene.h"

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{
	//�n�`����
	field = std::make_unique<Field>();
	//�v���C���[����
	player = std::make_unique<Player>();
	player->GetData()->SetAllActionInput(false); //�S�Ă̍s�����͂��󂯕t���Ȃ�

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



	//�`���[�g���A���p�����ݒ�
	player->GetData()->SetAllActionInput(false); //�����Ȃ����Ă���
	TextManager::Instance()->SentenceCreate(SentenceData::SentenceName::TUTORIAL_START);
	//�e�`���[�g���A���t�F�[�Y�̓��e���Z�b�g
	tutorialFunc.emplace_back([this] { return TutorialStartUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialRunUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialJumpUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialAttackUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialAvoidUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialFreeUpdate(); });
	//OK�X�v���C�g����
	okSprite = std::make_unique<OKSprite>();
	//�`���[�g���A���������s���I����^�C�}�[����
	tutorialActionClearTimer = std::make_unique<Engine::Timer>();

	//�s�����͐ݒ�
	actionInputConfig = std::make_unique<ActionInputConfig>();

	//�J�ڏ�����
	SceneChangeDirection::Instance()->Init();
}

void TutorialScene::Update()
{
	if (!isInputConfigMode) {
		//�Ó]�����������玟�̃V�[����
		if (isSceneChangeWait && SceneChangeDirection::Instance()->IsDirectionEnd()) {
			TitleScene* titleScene = new TitleScene;
			SceneManager::SetNextScene(titleScene);
		}

		//�e�`���[�g���A���t�F�[�Y�̓��e�X�V
		tutorialFunc[int(tutorialPhase)]();

		//�I�u�W�F�N�g�X�V
		player->Update();
		field->Update(player->GetData()->pos, camera->GetEye());

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

		if ((DirectInput::GetInstance()->TriggerKey(DIK_TAB) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_START))) {
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
	TextManager::Instance()->Update();
	SceneChangeDirection::Instance()->Update();
	okSprite->Update();
}

void TutorialScene::Draw(const int _cameraNum)
{
	player->Draw();
	field->Draw();
}

void TutorialScene::DrawLightView(const int _cameraNum)
{
	player->DrawLightView();
}

void TutorialScene::NonPostEffectDraw(const int _cameraNum)
{
	TextManager::Instance()->Draw();

	player->DrawSprite();

	okSprite->Draw();

	//���͐ݒ�`��
	if (isInputConfigMode) {
		actionInputConfig->Draw();
	}


	SceneChangeDirection::Instance()->Draw();
}

void TutorialScene::ImguiDraw()
{
}

void TutorialScene::FrameReset()
{
	player->FrameReset();
	field->FrameReset();
}

void TutorialScene::CollisionCheck()
{
}

void TutorialScene::TutorialStartUpdate()
{
	//���͕\�����I���΍ŏ��̐�����
	if (TextManager::Instance()->GetIsSentenceEnd(SentenceData::SentenceName::TUTORIAL_START)) {
		TextManager::Instance()->SentenceCreate(SentenceData::SentenceName::TUTORIAL_RUN);
		tutorialPhase = TutorialPhase::TUTORIAL_RUN;
	}
}

void TutorialScene::TutorialRunUpdate()
{
	//�`���[�g���A���������s�����N���A���Ă��Ȃ��ꍇ
	if (!isTutorialActionClear) {
		//�\������Ă���e�L�X�g������s�����ɕ\��������e�L�X�g�Ȃ�
		if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_RUN_ACTION_TEXT].text) {
			//�ړ����͂��s�\�Ȃ�\�ɂ��Ă���
			if (!player->GetData()->actionInput.isMove) {
				player->GetData()->actionInput.isMove = true;
			}

			//�ړ�����قǐ��������炵�Ă���
			if (GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash)) {
				//�����e�L�X�g�̐��������炵�Ă���
				int runNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				runNum--;
				//�w�肵���������炵��������`���[�g���A���������s���N���A
				if (runNum <= 0) {
					runNum = 0;
					isTutorialActionClear = true;

					//OK�X�v���C�g�`��J�n
					okSprite->DrawStart();
					//�v���C���[�̍s�����͂̎󂯕t�����֎~�ɂ���
					player->GetData()->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(runNum);
			}
		}
	}
	//�`���[�g���A���������s�����N���A���Ă���ꍇ
	else {
		TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_JUMP, SentenceData::SentenceName::TUTORIAL_JUMP);
	}
}

void TutorialScene::TutorialJumpUpdate()
{
	//�`���[�g���A���������s�����N���A���Ă��Ȃ��ꍇ
	if (!isTutorialActionClear) {
		//�\������Ă���e�L�X�g���W�����v�s�����ɕ\��������e�L�X�g�Ȃ�
		if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_JUMP_ACTION_TEXT].text) {
			//�ړ����͂��s�\�Ȃ�\�ɂ��Ă���
			if (!player->GetData()->actionInput.isMove) {
				player->GetData()->actionInput.isMove = true;
				player->GetData()->actionInput.isJump = true;
			}

			//�ړ�����قǐ��������炵�Ă���
			if (GameInputManager::TriggerInputAction(GameInputManager::Jump)) {
				//�����e�L�X�g�̐��������炵�Ă���
				int jumpNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				jumpNum--;
				//�w�肵���������炵��������`���[�g���A���������s���N���A
				if (jumpNum <= 0) {
					jumpNum = 0;
					isTutorialActionClear = true;

					//OK�X�v���C�g�`��J�n
					okSprite->DrawStart();
					//�v���C���[�̍s�����͂̎󂯕t�����֎~�ɂ���
					player->GetData()->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(jumpNum);
			}
		}
	}
	//�`���[�g���A���������s�����N���A���Ă���ꍇ
	else {
		TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_ATTACK, SentenceData::SentenceName::TUTORIAL_ATTACK);
	}
}

void TutorialScene::TutorialAttackUpdate()
{
	//�`���[�g���A���������s�����N���A���Ă��Ȃ��ꍇ
	if (!isTutorialActionClear) {
		//�\������Ă���e�L�X�g���U���s�����ɕ\��������e�L�X�g�Ȃ�
		if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_ATTACK_ACTION_TEXT].text) {
			//�ړ����͂��s�\�Ȃ�\�ɂ��Ă���
			if (!player->GetData()->actionInput.isMove) {
				player->GetData()->actionInput.isMove = true;
				player->GetData()->actionInput.isJump = true;
				player->GetData()->actionInput.isLightAttack = true;
				player->GetData()->actionInput.isStrongAttack = true;
			}

			//�ړ�����قǐ��������炵�Ă���
			if (GameInputManager::TriggerInputAction(GameInputManager::LightAttack)) {
				//�����e�L�X�g�̐��������炵�Ă���
				int destroyNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				destroyNum--;
				//�w�肵���������炵��������`���[�g���A���������s���N���A
				if (destroyNum <= 0) {
					destroyNum = 0;
					isTutorialActionClear = true;

					//OK�X�v���C�g�`��J�n
					okSprite->DrawStart();
					//�v���C���[�̍s�����͂̎󂯕t�����֎~�ɂ���
					player->GetData()->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(destroyNum);
			}
		}
	}
	//�`���[�g���A���������s�����N���A���Ă���ꍇ
	else {
		TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_AVOID, SentenceData::SentenceName::TUTORIAL_AVOID);
	}
}

void TutorialScene::TutorialAvoidUpdate()
{
	//�`���[�g���A���������s�����N���A���Ă��Ȃ��ꍇ
	if (!isTutorialActionClear) {
		//�\������Ă���e�L�X�g������s�����ɕ\��������e�L�X�g�Ȃ�
		if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_AVOID_ACTION_TEXT].text) {
			//�ړ����͂��s�\�Ȃ�\�ɂ��Ă���
			if (!player->GetData()->actionInput.isMove) {
				player->GetData()->actionInput.isMove = true;
				player->GetData()->actionInput.isJump = true;
				player->GetData()->actionInput.isAvoid = true;
				player->GetData()->actionInput.isBlink = true;
			}

			//�ړ�����قǐ��������炵�Ă���
			if (GameInputManager::TriggerInputAction(GameInputManager::Avoid_Blink_Dash)) {
				//�����e�L�X�g�̐��������炵�Ă���
				int avoidNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				avoidNum--;
				//�w�肵���������炵��������`���[�g���A���������s���N���A
				if (avoidNum <= 0) {
					avoidNum = 0;
					isTutorialActionClear = true;

					//OK�X�v���C�g�`��J�n
					okSprite->DrawStart();
					//�v���C���[�̍s�����͂̎󂯕t�����֎~�ɂ���
					player->GetData()->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(avoidNum);
			}
		}
	}
	//�`���[�g���A���������s�����N���A���Ă���ꍇ
	else {
		TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_FREE, SentenceData::SentenceName::TUTORIAL_FREE);
	}
}

void TutorialScene::TutorialFreeUpdate()
{
	//�\������Ă���e�L�X�g�����R�s�����ɕ\��������e�L�X�g�Ȃ�
	if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_FREE_ACTION_TEXT].text) {
		//�ړ����͂��s�\�Ȃ�\�ɂ��Ă���
		if (!player->GetData()->actionInput.isMove) {
			player->GetData()->SetAllActionInput(true);
		}
	}
}

void TutorialScene::TutorialActionClearAfterUpdate(TutorialPhase nextTutorialPhase, SentenceData::SentenceName nextTutorialSentenceName)
{
	//OK�X�v���C�g�̕\�����I��������N���A��^�C�}�[�X�V
	if (okSprite->GetIsDraw() && !okSprite->GetIsScaleChange()) {
		tutorialActionClearTimer->Update();
	}
	//�`���[�g���A���������s���I����^�C�}�[���w�肵�����ԂɂȂ����玟�̃`���[�g���A���t�F�[�Y��
	const int tutorialActionClearAfterTime = 60;
	if (*tutorialActionClearTimer.get() >= tutorialActionClearAfterTime) {
		isTutorialActionClear = false;
		tutorialActionClearTimer->Reset();
		okSprite->SetIsDraw(false);
		tutorialPhase = nextTutorialPhase;
		TextManager::Instance()->SentenceCreate(nextTutorialSentenceName);
	}
}
