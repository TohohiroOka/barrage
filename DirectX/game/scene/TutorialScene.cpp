#include "TutorialScene.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Scene/SceneManager.h"
#include "scene/TitleScene.h"
#include "scene/Scene1.h"
#include "cutscene/SceneChangeDirection.h"
#include "Audio/Audio.h"
#include "WindowApp.h"
#include "Object/3d/collider/Collision.h"
#include "TitleScene.h"
#include "../effect/AllHitEffect.h"


const DirectX::XMFLOAT3 TutorialScene::enemyBornPos = { 90, 10, 150 };

TutorialScene::~TutorialScene()
{
	AllHitEffect::Instance()->Finalize();
}

void TutorialScene::Initialize()
{
	AllHitEffect::Instance()->Initialize();

	//�n�`����
	field = std::make_unique<Field>();
	//�v���C���[����
	player = std::make_unique<Player>();
	player->GetData()->SetAllActionInput(false); //�S�Ă̍s�����͂��󂯕t���Ȃ�
	player->GetData()->isNoDead = true; //���ȂȂ��悤�ɂ��Ă���

	//�J��������
	GameCamera::SetPlayer(player.get());
	debugCamera = DebugCamera::Create({ 300, 40, 0 });
	camera = std::make_unique<TutorialCamera>();
	camera->SetAllActionInput(false); //�S�Ă̍s�����͂��󂯕t���Ȃ�
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
	tutorialFunc.emplace_back([this] { return TutorialCameraUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialAttackUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialAvoidUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialFreeUpdate(); });
	//�J�����`���[�g���A���t�F�[�Y�̓��e���Z�b�g
	tutorialCameraFunc.emplace_back([this] { return TutorialCameraText1(); });
	tutorialCameraFunc.emplace_back([this] { return TutorialCameraZoomEnemyBorn(); });
	tutorialCameraFunc.emplace_back([this] { return TutorialCameraEnemyBorn(); });
	tutorialCameraFunc.emplace_back([this] { return TutorialCameraReturnPos(); });
	tutorialCameraFunc.emplace_back([this] { return TutorialCameraText2Action(); });
	//OK�X�v���C�g����
	okSprite = std::make_unique<OKSprite>();
	//�`���[�g���A���������s���I����^�C�}�[����
	tutorialActionClearTimer = std::make_unique<Engine::Timer>();

	//�`���[�g���A���p�|�[�Y��ʐ���
	tutorialPause = std::make_unique<TutorialPause>();

	//�J�ڏ�����
	SceneChangeDirection::Instance()->Init();

	lockonUI = std::make_unique<LockonUI>();
	lockonUI->Init(camera.get());
}

void TutorialScene::Update()
{
	if (!tutorialPause->GetIsPauseDraw()) {
		//�e�`���[�g���A���t�F�[�Y�̓��e�X�V
		tutorialFunc[int(tutorialPhase)]();

		//�I�u�W�F�N�g�X�V
		player->Update();
		field->Update(player->GetData()->pos, camera->GetEye());
		if (tutorialEnemy) {
			tutorialEnemy->Update();
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
			if (tutorialEnemy) {
				lockonUI->Update(tutorialEnemy->GetObject3d()->GetPosition());
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

		//�|�[�Y��ʕ\��
		if (GameInputManager::TriggerInputAction(GameInputManager::Pause)) {
			tutorialPause->PauseStart();
		}

		if (!camera->GetIsLockon()) { lockonUI->EndLockOnDraw(); }

		TextManager::Instance()->UpdateSentence();
		okSprite->Update();
	}
	else {
		//�|�[�Y��ʕ\���I��
		if (GameInputManager::TriggerInputAction(GameInputManager::Pause)) {
			tutorialPause->PauseEnd();
		}
		//�Ó]�����������玟�̃V�[����
		if (isSceneChangeWait && SceneChangeDirection::Instance()->IsDirectionEnd()) {
			SceneManager::SetNextScene(changeScene);
		}
		//�I�����̑I�����I���Ă����
		if (TextManager::Instance()->GetIsChoiceEnd()) {
			//�I����0�ԂȂ�Q�[���V�[���J�n
			if (TextManager::Instance()->GetSelectNum(ChoicesData::ChoicesName::TUTORIAL_PAUSE_CHOICE) == 0) {
				tutorialPause->PauseEnd(); 
			}
			//�I����1�ԂȂ�^�C�g���V�[���ɖ߂�
			else if (TextManager::Instance()->GetSelectNum(ChoicesData::ChoicesName::TUTORIAL_PAUSE_CHOICE) == 1) {
				if (!isSceneChangeWait) {
					Scene1* gameScene = new Scene1;
					changeScene = gameScene;
					isSceneChangeWait = true;
					SceneChangeDirection::Instance()->PlayFadeOut();
				}
			}
			//�I����2�ԂȂ猳�ɖ߂�
			else if (TextManager::Instance()->GetSelectNum(ChoicesData::ChoicesName::TUTORIAL_PAUSE_CHOICE) == 2) {
				if (!isSceneChangeWait) {
					TitleScene* titleScene = new TitleScene;
					changeScene = titleScene;
					isSceneChangeWait = true;
					SceneChangeDirection::Instance()->PlayFadeOut();
				}
			}
		}
	}

	//�X�v���C�g�X�V	
	AllHitEffect::Instance()->Update();
	SceneChangeDirection::Instance()->Update();
	TextManager::Instance()->UpdateChoices();
	tutorialPause->Update();
}

void TutorialScene::Draw(const int _cameraNum)
{
	player->Draw();
	field->Draw();
	if (tutorialEnemy) {
		tutorialEnemy->Draw();
	}
	AllHitEffect::Instance()->Draw();
}

void TutorialScene::DrawLightView(const int _cameraNum)
{
	player->DrawLightView();
}

void TutorialScene::NonPostEffectDraw(const int _cameraNum)
{
	lockonUI->Draw();

	player->DrawSprite();


	TextManager::Instance()->DrawSentence();
	okSprite->Draw();

	tutorialPause->Draw();
	TextManager::Instance()->DrawChoices();

	SceneChangeDirection::Instance()->Draw();
}

void TutorialScene::ImguiDraw()
{
}

void TutorialScene::FrameReset()
{
	player->FrameReset();
	field->FrameReset();
	tutorialEnemy->FrameReset();
}

void TutorialScene::CollisionCheck()
{
	//�G�֌W�̓����蔻�肵���Ȃ��̂ŁA�G�����݂��Ȃ���Δ�����
	if (!tutorialEnemy) { return; }

#pragma region �v���C���[�ƓG�̏Փ˔���
	{
		const Vector3 ppos = player->GetData()->pos;
		Sphere playerSphere;
		playerSphere.center = { ppos.x, ppos.y, ppos.z, 1.0f };
		playerSphere.radius = player->GetFbxObject()->GetScale().x;

		Sphere enemySphere;
		enemySphere.center = { tutorialEnemy->GetObject3d()->GetPosition().x, tutorialEnemy->GetObject3d()->GetPosition().y, tutorialEnemy->GetObject3d()->GetPosition().z, 1.0f };
		enemySphere.radius = tutorialEnemy->GetObject3d()->GetScale().x;

		XMVECTOR inter;
		XMVECTOR reject;
		if (Collision::CheckSphere2Sphere(playerSphere, enemySphere, &inter, &reject)) {
			//�v���C���[�������߂�
			player->PushBack(reject);
		}
	}
#pragma endregion

#pragma region �v���C���[�̍U���ƓG�̏Փ˔���
	{
		//�v���C���[�̍U��������ꍇ�̂ݔ��� 
		if (player->GetData()->attackAction) {
			Sphere enemySphere;
			enemySphere.center = { tutorialEnemy->GetObject3d()->GetPosition().x, tutorialEnemy->GetObject3d()->GetPosition().y, tutorialEnemy->GetObject3d()->GetPosition().z, 1.0f };
			enemySphere.radius = tutorialEnemy->GetObject3d()->GetScale().x;

			Capsule attackCapsule;
			attackCapsule.startPosition = player->GetData()->attackAction->GetAttackCollisionData().startPosition;
			attackCapsule.endPosition = player->GetData()->attackAction->GetAttackCollisionData().endPosition;
			attackCapsule.radius = player->GetData()->attackAction->GetAttackCollisionData().radius;

			float dist;
			Vector3 collisionPos;
			if (Collision::CheckSphereCapsule(enemySphere, attackCapsule, &dist, &collisionPos)) {

				//�G�Ƀq�b�g�G�t�F�N�g���o��
				AllHitEffect::Instance()->AddParticle(collisionPos);

				//�U���������L���ɂ��Ă�����_���[�W��^����
				if (player->GetData()->attackAction->GetIsCollisionValid()) {
					//���t���[�����i�q�b�g����̂�h�����߁A���̍U���̏Փ˔����off�ɂ��Ă����B
					player->GetData()->attackAction->AttackCollision();

					//�_���[�W
					tutorialEnemy->Damage();

					//�q�b�g�X�g�b�v
					const int hitStopFrame = 5;
					GameHelper::Instance()->SetSlow(0, hitStopFrame);
					//�U���q�b�g���Đ�
					Audio::Instance()->SoundPlayWava(Sound::SoundName::attack_hit, false, 0.1f);
				}
			}
		}
	}
#pragma endregion

#pragma region �v���C���[�ƓG�̍U���̏Փ˔���
	//���Ƌ�
	{
		std::vector<Sphere> bossAttackDatas;
		tutorialEnemy->GetAttackCollision(bossAttackDatas);

		const Vector3 ppos = player->GetData()->pos;
		Sphere playerSphere;
		playerSphere.center = { ppos.x, ppos.y, ppos.z, 1.0f };
		playerSphere.radius = player->GetFbxObject()->GetScale().x;

		//�v���C���[�_���[�W����p
		//�v���C���[������܂��̓u�����N�����Ă��Ȃ���ΏՓ˔���
		if (!player->GetData()->isNoDamage) {
			int num = -1;
			for (auto& bossAttackData : bossAttackDatas) {
				num++;
				if (Collision::CheckSphere2Sphere(playerSphere, bossAttackData)) {
					Vector3 knockbackVec = ppos - Vector3{ bossAttackData.center.m128_f32[0], bossAttackData.center.m128_f32[1], bossAttackData.center.m128_f32[2] };

					//�_���[�W����
					player->Damage(1, knockbackVec, 5, 1, true);
					camera->ShakeStart(10, 10);
					tutorialEnemy->DeleteBullet({ num });
					//�_���[�W���Đ�
					Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

					break;
				}
			}
		}

		//�v���C���[����𔻒�������ԂȂ��𔻒������
		if (player->GetData()->isEnemyAttackAvoidJudge) {
			int num = -1;
			for (auto& bossAttackData : bossAttackDatas) {
				num++;
				const float avoidCheckRadiusMulti = 75.0f;
				bossAttackData.radius *= avoidCheckRadiusMulti;
				if (Collision::CheckSphere2Sphere(playerSphere, bossAttackData)) {
					//�������
					player->EnemyAttackAvoid();

					break;
				}
			}
		}
	}
#pragma endregion

#pragma region �J�����̃��b�N�I���^�[�Q�b�g�ݒ�
	{
		//�J���������b�N�I���^�[�Q�b�g�����o���Ă���ꍇ�̂ݔ���
		if (camera->GetisLockonStart()) {
			//�G���W
			const XMFLOAT2 pos = tutorialEnemy->GetObject3d()->GetScreenPosition();

			//�G�̃X�N���[�����W�����o�Ώ۔͈͓��Ȃ珈��
			const float targetScreenDistance = 100;
			const bool isInsideTargetScreen = (pos.x <= WindowApp::GetWindowWidth() - targetScreenDistance && pos.x >= targetScreenDistance &&
				pos.y <= WindowApp::GetWindowHeight() - targetScreenDistance && pos.y >= targetScreenDistance);
			if (isInsideTargetScreen) {
				//���b�N�I���Ώۂ��m�肳����
				camera->Lockon(tutorialEnemy->GetObject3d());
				//���b�N�I��UI�\��
				lockonUI->StartLockOnAnimation();
			}
		}
	}
#pragma endregion
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
			//�J������]���͂��s�\�Ȃ�\�ɂ��Ă���
			if (!camera->actionInput.isCameraMove) {
				camera->actionInput.isCameraMove = true;
			}

			//�ړ�����قǐ��������炵�Ă���
			if (player->GetData()->isDash) {
				//�����e�L�X�g�̐��������炵�Ă���
				float runNum = (float)TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				runNum -= player->GetData()->moveSpeed;
				//�w�肵���������炵��������`���[�g���A���������s���N���A
				if (runNum <= 0) {
					runNum = 0;
					isTutorialActionClear = true;

					//OK�X�v���C�g�`��J�n
					okSprite->DrawStart();
					//�v���C���[�̍s�����͂̎󂯕t�����֎~�ɂ���
					player->GetData()->SetAllActionInput(false);
					//�J�����̓��͂̎󂯕t�����֎~�ɂ���
					camera->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber((int)runNum);
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
			//�J������]���͂��s�\�Ȃ�\�ɂ��Ă���
			if (!camera->actionInput.isCameraMove) {
				camera->actionInput.isCameraMove = true;
			}

			//�W�����v����قǐ��������炵�Ă���
			if (player->GetData()->action == PlayerActionName::JUMP) {
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
					//�J�����̓��͂̎󂯕t�����֎~�ɂ���
					camera->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(jumpNum);
			}
		}
	}
	//�`���[�g���A���������s�����N���A���Ă���ꍇ
	else {
		if (XInputManager::GetInstance()->ControllerConnectCheck()) { TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_CAMERA, SentenceData::SentenceName::TUTORIAL_CAMERA_1_PAD); }
		else { TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_CAMERA, SentenceData::SentenceName::TUTORIAL_CAMERA_1_KEY); }
	}
}

void TutorialScene::TutorialCameraUpdate()
{
	//�e�`���[�g���A���t�F�[�Y�̓��e�X�V
	tutorialCameraFunc[int(tutorialCameraPhasePhase)]();
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
			//�J�������͂��s�\�Ȃ�\�ɂ��Ă���
			if (!camera->actionInput.isCameraMove) {
				camera->SetAllActionInput(true);
			}

			//�U���Ń_���[�W�𓖂����x�ɐ��������炵�Ă���
			if (tutorialEnemy->GetIsDamage()) {
				//�����e�L�X�g�̐��������炵�Ă���
				int damageNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				damageNum--;
				//�w�肵���������炵��������`���[�g���A���������s���N���A
				if (damageNum <= 0) {
					damageNum = 0;
					isTutorialActionClear = true;

					//OK�X�v���C�g�`��J�n
					okSprite->DrawStart();
					//�v���C���[�̍s�����͂̎󂯕t�����֎~�ɂ���
					player->GetData()->SetAllActionInput(false);
					//�J�����̓��͂̎󂯕t�����֎~�ɂ���
					camera->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(damageNum);
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
			//�J�������͂��s�\�Ȃ�\�ɂ��Ă���
			if (!camera->actionInput.isCameraMove) {
				camera->SetAllActionInput(true);
			}
			//�G�̒e���˃t���O�𗧂Ă�
			if (!tutorialEnemy->GetIsBulletShot()) {
				tutorialEnemy->SetIsBulletShot(true);
			}

			//�������قǐ��������炵�Ă���
			if (player->GetData()->isEnemyAttackAvoid) {
				//�����e�L�X�g�̐��������炵�Ă���
				int avoidNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				avoidNum--;
				player->GetData()->isEnemyAttackAvoid = false;
				//�w�肵���������炵��������`���[�g���A���������s���N���A
				if (avoidNum <= 0) {
					avoidNum = 0;
					isTutorialActionClear = true;

					//OK�X�v���C�g�`��J�n
					okSprite->DrawStart();
					//�v���C���[�̍s�����͂̎󂯕t�����֎~�ɂ���
					player->GetData()->SetAllActionInput(false);
					//�J�����̓��͂̎󂯕t�����֎~�ɂ���
					camera->SetAllActionInput(false);
					//�G�̒e���˃t���O�����낷
					tutorialEnemy->SetIsBulletShot(false);
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
		//�J������]���s�\�Ȃ�\�ɂ��Ă���
		if (!camera->actionInput.isCameraMove) {
			camera->SetAllActionInput(true);
		}
		//�G�̒e���˃t���O�𗧂Ă�
		if (!tutorialEnemy->GetIsBulletShot()) {
			tutorialEnemy->SetIsBulletShot(true);
		}
	}
}

void TutorialScene::TutorialCameraText1()
{
	//�J������������1���I�����Ă��Ȃ���Δ�����
	if (!(TextManager::Instance()->GetIsSentenceEnd(SentenceData::SentenceName::TUTORIAL_CAMERA_1_KEY) ||
		TextManager::Instance()->GetIsSentenceEnd(SentenceData::SentenceName::TUTORIAL_CAMERA_1_PAD))) {
		return;
	}

	//�e�L�X�g�\���I��
	TextManager::Instance()->SentenceDrawEnd();

	//�J������G�������s����ʒu�܂ňړ�������
	camera->SetZoomTargetPos(enemyBornPos);
	camera->ChangePhase(TutorialCamera::TutorialCameraPhase::ZOOM_ENEMY_BORN);

	//���̃t�F�[�Y��
	tutorialCameraPhasePhase = TutorialCameraPhasePhase::TUTORIAL_CAMERA_ZOOM_ENEMY_BORN;
}

void TutorialScene::TutorialCameraZoomEnemyBorn()
{
	//�J�����̃Y�[�����I�����Ă��Ȃ���Δ�����
	if (!(camera->GetTutorialCameraPhase() == TutorialCamera::TutorialCameraPhase::ZOOM_ENEMY_BORN)) { return; }
	if (!camera->GetIsPhaseActionEnd()) { return; }

	//���̃t�F�[�Y��
	tutorialCameraPhasePhase = TutorialCameraPhasePhase::TUTORIAL_CAMERA_ENEMY_BORN;
}

void TutorialScene::TutorialCameraEnemyBorn()
{
	//�G�������O�̂Ƃ�
	if (!tutorialEnemy) {
		enemyBornTimer++;
		if (enemyBornTimer <= 30) { return; }

		tutorialEnemy = std::make_unique<TutorialEnemy>(enemyBornPos, player->GetData());

		enemyBornTimer = 0;
	}
	else {
		enemyBornTimer++;
		if (enemyBornTimer <= 30) { return; }

		//�J���������̈ʒu�܂Ŗ߂�
		camera->ChangePhase(TutorialCamera::TutorialCameraPhase::ZOOM_END_RETURN);

		//���̃t�F�[�Y��
		tutorialCameraPhasePhase = TutorialCameraPhasePhase::TUTORIAL_CAMERA_RETURN_POS;
	}
}

void TutorialScene::TutorialCameraReturnPos()
{
	//�J�����̌��ɖ߂鏈�����I�����Ă��Ȃ���Δ�����
	if (!(camera->GetTutorialCameraPhase() == TutorialCamera::TutorialCameraPhase::ZOOM_END_RETURN)) { return; }
	if (!camera->GetIsPhaseActionEnd()) { return; }

	//�J������������2�𐶐�����
	TextManager::Instance()->SentenceCreate(SentenceData::SentenceName::TUTORIAL_CAMERA_2);

	//�J������Ԃ�ʏ��Ԃɖ߂�
	camera->ChangePhase(TutorialCamera::TutorialCameraPhase::NORMAL);

	//���̃t�F�[�Y��
	tutorialCameraPhasePhase = TutorialCameraPhasePhase::TUTORIAL_CAMERA_TEXT_2_ACTION;
}

void TutorialScene::TutorialCameraText2Action()
{
	//�`���[�g���A���������s�����N���A���Ă��Ȃ��ꍇ
	if (!isTutorialActionClear) {
		//�\������Ă���e�L�X�g�����b�N�I���s�����ɕ\��������e�L�X�g�Ȃ�
		if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_CAMERA_ACTION_TEXT].text) {
			//�ړ����͂��s�\�Ȃ�\�ɂ��Ă���
			if (!player->GetData()->actionInput.isMove) {
				player->GetData()->actionInput.isMove = true;
				player->GetData()->actionInput.isJump = true;
			}
			//�J�������͂��s�\�Ȃ�\�ɂ��Ă���
			if (!camera->actionInput.isCameraMove) {
				camera->SetAllActionInput(true);
			}

			//���b�N�I������قǐ��������炵�Ă���
			if (camera->GetIsLockon()) {
				//�����e�L�X�g�̐��������炵�Ă���
				int lockonNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				lockonNum--;
				//�w�肵���������炵��������`���[�g���A���������s���N���A
				if (lockonNum <= 0) {
					lockonNum = 0;
					isTutorialActionClear = true;

					//OK�X�v���C�g�`��J�n
					okSprite->DrawStart();
					//�v���C���[�̍s�����͂̎󂯕t�����֎~�ɂ���
					player->GetData()->SetAllActionInput(false);
					//�J�����̓��͂̎󂯕t�����֎~�ɂ���
					camera->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(lockonNum);
			}
		}
	}
	//�`���[�g���A���������s�����N���A���Ă���ꍇ
	else {
		TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_ATTACK, SentenceData::SentenceName::TUTORIAL_ATTACK);
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
