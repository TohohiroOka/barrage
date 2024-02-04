#include "Scene1.h"
#include "Scene/SceneManager.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "Object/2d/DebugText.h"
#include "WindowApp.h"
#include <imgui.h>
#include "Object/3d/collider/Collision.h"
#include "Object/3d/collider/MeshCollider.h"
#include "Object/3d/collider/CollisionAttribute.h"
#include "GameHelper.h"
#include "scene/TitleScene.h"
#include "effect/BulletEffect.h"
#include "scene/OnStageTest.h"
#include "cutscene/SceneChangeDirection.h"
#include "../effect/AllHitEffect.h"

using namespace DirectX;

const std::array<XMFLOAT4, 2> COLOR = { XMFLOAT4{ 0.0f,0.0f,0.8f,1.0f } ,{ 0.8f,0.0f,0.0f,1.0f } };

Scene1::~Scene1()
{
	AllHitEffect::Instance()->Finalize();
}

void Scene1::Initialize()
{
	TextureManager::LoadTexture("gauge", "Resources/SpriteTexture/gauge.png");

	AllHitEffect::Instance()->Initialize();

	//�n�`����
	field = std::make_unique<Field>();

	player = std::make_unique<Player>();

	GameCamera::SetPlayer(player.get());
	camera = std::make_unique<GameCamera>();
	player->SetGameCamera(camera.get());

	//�e�p�����J����������
	lightCamera = std::make_unique<LightCamera>(Vector3{ 205, 200, 204 }, Vector3{ 205, 0, 205 });
	const float projectionSize = 1.5f;
	lightCamera->SetProjectionNum({ projectionSize * (float)WindowApp::GetWindowWidth() / 5, projectionSize * (float)WindowApp::GetWindowHeight() / 5 },
		{ -projectionSize * (float)WindowApp::GetWindowWidth() / 5, -projectionSize * (float)WindowApp::GetWindowHeight() / 5 });

	Base3D::SetCamera(camera.get());
	Base3D::SetLightCamera(lightCamera.get());

	boss = std::make_unique<Boss1>();

	ParticleManager::SetCamera(camera.get());

	//stop = false;

	gameoverUi = std::make_unique<GameOver>();
	gameoverUi->Initialize();
	gameoverUi->SetPlayerObject(player->GetFbxObject());

	BulletEffect::LoadResources();

	defeatDirection = std::make_unique<Boss1Defeat>();
	defeatDirection->Init();

	SceneChangeDirection::Instance()->Init();

	lockonUI = std::make_unique<LockonUI>();
	lockonUI->Init(camera.get());

	screenCut = std::make_unique<ScreenCut>();

	pauseScene = std::make_unique<PauseScene>();
	pauseScene->Init();
}

void Scene1::Update()
{
	DirectInput* input = DirectInput::GetInstance();

	//GameHelper::Instance()->SetStop(stop);

	//�G���o�p�̃N���X
	screenCut->Update();

	//���b�N�I����������
	if (boss->GetBaseAction2()) {
		if (!boss->GetBaseAction2()->SetCameraTarget()) {
			camera->LockonEnd(true);
		} else {
			//���b�N�I���Ώۂ��m�肳����
			camera->Lockon(boss->GetCenter());
			//���b�N�I��UI�\��
			lockonUI->StartLockOnAnimation();
		}
	}

	if (!pauseScene->GetIsPause()) {
		//���j���o�Đ�
		//�f�o�b�O�p�Đ�
		if (DirectInput::GetInstance()->TriggerKey(DIK_H)) {
			defeatDirection->StartDefeatDirection(boss->GetBaseModel()->GetPosition());
		}
		if (boss->GetBossIsAlive()) { defeatDirection->StartDefeatDirection(boss->GetBaseModel()->GetPosition()); }

		CollisionCheck();

		//�J�����X�V
		camera->Update();
		lockonUI->Update(boss->GetCenter()->GetPosition());
		if (DirectInput::GetInstance()->TriggerKey(DIK_9)) {
			OnStageTestScene* testScene = new OnStageTestScene;
			SceneManager::SetNextScene(testScene);
		}
		lightCamera->Update();

		//�I�u�W�F�N�g�X�V
		field->Update(player->GetData()->pos, camera->GetEye());
		player->Update();
		boss->SetTargetPos(player->GetData()->pos);
		boss->Update();

		//�f�o�b�O�p�V�[���؂�ւ�
		if (DirectInput::GetInstance()->ReleaseKey(DIK_1)) {
			TitleScene* titleScene = new TitleScene;
			SceneManager::SetNextScene(titleScene);
		}

		gameoverUi->Update();
		//�̗�0�ŃQ�[���I�[�o�[�\��
		//�f�o�b�O�p�Q�[���I�[�o�[�\��
		if (DirectInput::GetInstance()->TriggerKey(DIK_F4)) { gameoverUi->ResetGameOverUI(); }
		if (DirectInput::GetInstance()->TriggerKey(DIK_4)) { gameoverUi->StartGameOverUI(); }
		if (player->GetData()->isDead && !gameoverUi->GetIsGameOver()) { 
			gameoverUi->StartGameOverUI();

			//�J�������b�N�I�����������đ���o���Ȃ�����
			camera->LockonEnd(false);
			camera->SetAllActionInput(false);
		}

		if (!camera->GetIsLockon()) { lockonUI->EndLockOnDraw(); }

		//�|�[�X����
		pauseScene->InPause();
	}
	else {
		pauseScene->Update();
	}

	defeatDirection->Update();
	if (defeatDirection->GetDirectionEnd() && isSceneChangeWait == false) {
		isSceneChangeWait = true;
		SceneChangeDirection::Instance()->PlayFadeOut();
	}
	if (pauseScene->GetIsChangeScene() && isSceneChangeWait == false) {
		isSceneChangeWait = true;
		SceneChangeDirection::Instance()->PlayFadeOut();
	}
	if (isSceneChangeWait && SceneChangeDirection::Instance()->IsDirectionEnd()) {
		TitleScene* titleScene = new TitleScene;
		SceneManager::SetNextScene(titleScene);
	}

	AllHitEffect::Instance()->Update();

	SceneChangeDirection::Instance()->Update();
}

void Scene1::Draw(const int _cameraNum)
{
	//gobject->ColliderDraw();

	player->Draw();
	boss->Draw();
	AllHitEffect::Instance()->Draw();

	defeatDirection->Draw();
	field->Draw();
}

void Scene1::DrawLightView(const int _cameraNum)
{
	player->DrawLightView();
	boss->DrawLightView();
}

void Scene1::NonPostEffectDraw(const int _cameraNum)
{
	screenCut->Draw();

	//�X�v���C�g
	if (_cameraNum == 0) {
		DebugText::GetInstance()->DrawAll();
		//sprite->Draw();
	}

	lockonUI->Draw();

	player->DrawSprite();
	boss->DrawSprite();
	gameoverUi->Draw();

	defeatDirection->Draw2D();

	pauseScene->Draw();

	SceneChangeDirection::Instance()->Draw();
}

void Scene1::ImguiDraw()
{
	ImGui::Begin("debug imgui");

	ImGui::SetWindowSize(ImVec2(300, 300), ImGuiCond_::ImGuiCond_FirstUseEver);
	player->ImguiDraw();

	ImGui::SliderInt("HitStopNum", &hitStopFrame, 0, 20);

	XMFLOAT2 mousePos = DirectInput::GetInstance()->GetMousePoint();
	ImGui::Text("mouse : x %f:y %f", mousePos.x, mousePos.y);
	ImGui::End();
}

void Scene1::FrameReset()
{
	field->FrameReset();
	player->FrameReset();
	boss->FrameReset();
}

void Scene1::CollisionCheck()
{
#pragma region �v���C���[�ƓG�̏Փ˔���
	{
		const Vector3 ppos = player->GetData()->pos;
		Sphere playerSphere;
		playerSphere.center = { ppos.x, ppos.y, ppos.z, 1.0f };
		playerSphere.radius = player->GetFbxObject()->GetScale().x;

		Sphere enemySphere;
		enemySphere.center = { boss->GetBaseModel()->GetPosition().x, boss->GetBaseModel()->GetPosition().y, boss->GetBaseModel()->GetPosition().z, 1.0f };
		enemySphere.radius = boss->GetHitScale();

		XMVECTOR inter;
		XMVECTOR reject;
		if (Collision::CheckSphere2Sphere(playerSphere, enemySphere, &inter, &reject)) {
			//�v���C���[�������߂�
			player->PushBack(reject);
		}
	}
#pragma endregion

#pragma region �v���C���[�ƓG�̍U���̏Փ˔���
	//���S���Ă��Ȃ���Δ��肷��
	if (!player->GetData()->isDead) {
		BaseAction* action = boss->GetBaseAction();
		//�v���C���[������܂��̓u�����N�����Ă��Ȃ���ΏՓ˔���
		//�U���̔�����s��Ȃ����ԂȂ画������Ȃ�
		if ((!player->GetData()->isNoDamage) && action->GetIsCollision()) {
			const Vector3 ppos = player->GetData()->pos;
			Sphere playerSphere;
			playerSphere.center = { ppos.x, ppos.y, ppos.z, 1.0f };
			playerSphere.radius = player->GetFbxObject()->GetScale().x;

			//���ƃ{�b�N�X
			if (action->GetUseCollision() == BaseAction::UseCollision::box) {
				std::vector<Box> bossAttackDatas;
				action->GetAttackCollisionBox(bossAttackDatas);

				int num = -1;
				for (auto& i : bossAttackDatas) {
					num++;
					if (Collision::CheckSphere2Box(playerSphere, i)) {
						Vector3 knockbackVec = ppos - (Vector3)i.point1;

						//�_���[�W�̏��擾
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//�_���[�W����
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//�_���[�W���Đ�
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
			//���Ƌ�
			else if (action->GetUseCollision() == BaseAction::UseCollision::sphere) {
				std::vector<Sphere> bossAttackDatas;
				action->GetAttackCollisionSphere(bossAttackDatas);

				int num = -1;
				for (auto& i : bossAttackDatas) {
					num++;
					if (Collision::CheckSphere2Sphere(playerSphere, i)) {
						Vector3 knockbackVec = ppos - Vector3{ i.center.m128_f32[0],i.center.m128_f32[1], i.center.m128_f32[2] };

						//�_���[�W�̏��擾
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//�_���[�W����
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//�_���[�W���Đ�
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
			//���ƃJ�v�Z��
			else if (action->GetUseCollision() == BaseAction::UseCollision::capsule) {
				std::vector<Capsule> bossAttackDatas;
				action->GetAttackCollisionCapsule(bossAttackDatas);

				int num = -1;
				for (auto& i : bossAttackDatas) {
					num++;
					float dist;
					Vector3 collisionPos;
					if (Collision::CheckSphereCapsule(playerSphere, i, &dist, &collisionPos)) {
						Vector3 knockbackVec = ppos - i.startPosition;

						//�_���[�W�̏��擾
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//�_���[�W����
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//�_���[�W���Đ�
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
		}
	}
#pragma endregion

#pragma region �v���C���[�ƓG�̍U���̏Փ˔���2
	if (!player->GetData()->isDead && boss->GetBaseAction2()) {
		BaseAction* action = boss->GetBaseAction2();
		//�v���C���[������܂��̓u�����N�����Ă��Ȃ���ΏՓ˔���
		//�U���̔�����s��Ȃ����ԂȂ画������Ȃ�
		if ((!player->GetData()->isNoDamage) && action->GetIsCollision()) {
			const Vector3 ppos = player->GetData()->pos;
			Sphere playerSphere;
			playerSphere.center = { ppos.x, ppos.y, ppos.z, 1.0f };
			playerSphere.radius = player->GetFbxObject()->GetScale().x;

			//���ƃ{�b�N�X
			if (action->GetUseCollision() == BaseAction::UseCollision::box) {
				std::vector<Box> bossAttackDatas;
				action->GetAttackCollisionBox(bossAttackDatas);

				int num = -1;
				for (auto& i : bossAttackDatas) {
					num++;
					if (Collision::CheckSphere2Box(playerSphere, i)) {
						Vector3 knockbackVec = ppos - (Vector3)i.point1;

						//�_���[�W�̏��擾
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//�_���[�W����
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//�_���[�W���Đ�
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
			//���Ƌ�
			else if (boss->GetBaseAction2()->GetUseCollision() == BaseAction::UseCollision::sphere) {
				std::vector<Sphere> bossAttackDatas;
				action->GetAttackCollisionSphere(bossAttackDatas);

				int num = -1;
				for (auto& i : bossAttackDatas) {
					num++;
					if (Collision::CheckSphere2Sphere(playerSphere, i)) {
						Vector3 knockbackVec = ppos - Vector3{ i.center.m128_f32[0], i.center.m128_f32[1], i.center.m128_f32[2] };

						//�_���[�W�̏��擾
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//�_���[�W����
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//�_���[�W���Đ�
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
			//���ƃJ�v�Z��
			else if (action->GetUseCollision() == BaseAction::UseCollision::capsule) {
				std::vector<Capsule> bossAttackDatas;
				action->GetAttackCollisionCapsule(bossAttackDatas);

				int num = -1;
				for (auto& i : bossAttackDatas) {
					num++;
					float dist;
					Vector3 collisionPos;
					if (Collision::CheckSphereCapsule(playerSphere, i, &dist, &collisionPos)) {
						Vector3 knockbackVec = ppos - i.startPosition;

						//�_���[�W�̏��擾
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//�_���[�W����
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//�_���[�W���Đ�
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
		}
	}
#pragma endregion

#pragma region �v���C���[�̍U���ƓG�̏Փ˔���
	{
		//�v���C���[�̍U��������ꍇ�̂ݔ��� 
		if (player->GetData()->attackAction) {
			Sphere enemySphere;
			enemySphere.center = { boss->GetBaseModel()->GetPosition().x, boss->GetBaseModel()->GetPosition().y, boss->GetBaseModel()->GetPosition().z, 1.0f };
			enemySphere.radius = boss->GetHitScale();

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
					//�G�Ƀ_���[�W
					boss->Damage(player->GetData()->attackAction->GetAttackCollisionData().power);

					//���t���[�����i�q�b�g����̂�h�����߁A���̍U���̏Փ˔����off�ɂ��Ă����B
					player->GetData()->attackAction->AttackCollision();

					//�q�b�g�X�g�b�v
					GameHelper::Instance()->SetSlow(0, hitStopFrame);
					//�U���q�b�g���Đ�
					Audio::Instance()->SoundPlayWava(Sound::SoundName::attack_hit, false, 0.1f);
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
			XMFLOAT2 pos = boss->GetCenter()->GetScreenPosition();

			//�G�̃X�N���[�����W�����o�Ώ۔͈͓��Ȃ珈��
			const float targetScreenDistance = 100;
			const bool isInsideTargetScreen = (pos.x <= WindowApp::GetWindowWidth() - targetScreenDistance && pos.x >= targetScreenDistance &&
				pos.y <= WindowApp::GetWindowHeight() - targetScreenDistance && pos.y >= targetScreenDistance);

			//�J�����ƓG�������������Ă����珈��
			XMMATRIX cameraFrontMatWorld;
			XMMATRIX matTrans = XMMatrixTranslation(0, 0, 1);
			cameraFrontMatWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
			cameraFrontMatWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f
			cameraFrontMatWorld *= camera->GetMatWorld();

			Vector3 cameraFrontVec = camera->GetEye() - Vector3{ cameraFrontMatWorld.r[3].m128_f32[0], cameraFrontMatWorld.r[3].m128_f32[1], cameraFrontMatWorld.r[3].m128_f32[2] };
			cameraFrontVec.normalize();
			Vector3 cameraEnemyVec = camera->GetEye() - Vector3(boss->GetCenter()->GetPosition());
			cameraEnemyVec.normalize();

			float dot = cameraFrontVec.dot(cameraEnemyVec);
			bool isCameraEnemyFront = (dot >= 0);

			if (isInsideTargetScreen && isCameraEnemyFront) {
				//���b�N�I���Ώۂ��m�肳����
				camera->Lockon(boss->GetCenter());
				//���b�N�I��UI�\��
				lockonUI->StartLockOnAnimation();
			}
		}
	}
#pragma endregion
}
