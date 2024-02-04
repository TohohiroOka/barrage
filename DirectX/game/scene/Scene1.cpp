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

	//地形生成
	field = std::make_unique<Field>();

	player = std::make_unique<Player>();

	GameCamera::SetPlayer(player.get());
	camera = std::make_unique<GameCamera>();
	player->SetGameCamera(camera.get());

	//影用光源カメラ初期化
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

	//敵演出用のクラス
	screenCut->Update();

	//ロックオン強制解除
	if (boss->GetBaseAction2()) {
		if (!boss->GetBaseAction2()->SetCameraTarget()) {
			camera->LockonEnd(true);
		} else {
			//ロックオン対象を確定させる
			camera->Lockon(boss->GetCenter());
			//ロックオンUI表示
			lockonUI->StartLockOnAnimation();
		}
	}

	if (!pauseScene->GetIsPause()) {
		//撃破演出再生
		//デバッグ用再生
		if (DirectInput::GetInstance()->TriggerKey(DIK_H)) {
			defeatDirection->StartDefeatDirection(boss->GetBaseModel()->GetPosition());
		}
		if (boss->GetBossIsAlive()) { defeatDirection->StartDefeatDirection(boss->GetBaseModel()->GetPosition()); }

		CollisionCheck();

		//カメラ更新
		camera->Update();
		lockonUI->Update(boss->GetCenter()->GetPosition());
		if (DirectInput::GetInstance()->TriggerKey(DIK_9)) {
			OnStageTestScene* testScene = new OnStageTestScene;
			SceneManager::SetNextScene(testScene);
		}
		lightCamera->Update();

		//オブジェクト更新
		field->Update(player->GetData()->pos, camera->GetEye());
		player->Update();
		boss->SetTargetPos(player->GetData()->pos);
		boss->Update();

		//デバッグ用シーン切り替え
		if (DirectInput::GetInstance()->ReleaseKey(DIK_1)) {
			TitleScene* titleScene = new TitleScene;
			SceneManager::SetNextScene(titleScene);
		}

		gameoverUi->Update();
		//体力0でゲームオーバー表示
		//デバッグ用ゲームオーバー表示
		if (DirectInput::GetInstance()->TriggerKey(DIK_F4)) { gameoverUi->ResetGameOverUI(); }
		if (DirectInput::GetInstance()->TriggerKey(DIK_4)) { gameoverUi->StartGameOverUI(); }
		if (player->GetData()->isDead && !gameoverUi->GetIsGameOver()) { 
			gameoverUi->StartGameOverUI();

			//カメラロックオンを解除して操作出来なくする
			camera->LockonEnd(false);
			camera->SetAllActionInput(false);
		}

		if (!camera->GetIsLockon()) { lockonUI->EndLockOnDraw(); }

		//ポース判定
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

	//スプライト
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
#pragma region プレイヤーと敵の衝突判定
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
			//プレイヤーを押し戻す
			player->PushBack(reject);
		}
	}
#pragma endregion

#pragma region プレイヤーと敵の攻撃の衝突判定
	//死亡していなければ判定する
	if (!player->GetData()->isDead) {
		BaseAction* action = boss->GetBaseAction();
		//プレイヤーが回避またはブリンクをしていなければ衝突判定
		//攻撃の判定を行わない時間なら判定を取らない
		if ((!player->GetData()->isNoDamage) && action->GetIsCollision()) {
			const Vector3 ppos = player->GetData()->pos;
			Sphere playerSphere;
			playerSphere.center = { ppos.x, ppos.y, ppos.z, 1.0f };
			playerSphere.radius = player->GetFbxObject()->GetScale().x;

			//球とボックス
			if (action->GetUseCollision() == BaseAction::UseCollision::box) {
				std::vector<Box> bossAttackDatas;
				action->GetAttackCollisionBox(bossAttackDatas);

				int num = -1;
				for (auto& i : bossAttackDatas) {
					num++;
					if (Collision::CheckSphere2Box(playerSphere, i)) {
						Vector3 knockbackVec = ppos - (Vector3)i.point1;

						//ダメージの情報取得
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//ダメージ処理
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//ダメージ音再生
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
			//球と球
			else if (action->GetUseCollision() == BaseAction::UseCollision::sphere) {
				std::vector<Sphere> bossAttackDatas;
				action->GetAttackCollisionSphere(bossAttackDatas);

				int num = -1;
				for (auto& i : bossAttackDatas) {
					num++;
					if (Collision::CheckSphere2Sphere(playerSphere, i)) {
						Vector3 knockbackVec = ppos - Vector3{ i.center.m128_f32[0],i.center.m128_f32[1], i.center.m128_f32[2] };

						//ダメージの情報取得
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//ダメージ処理
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//ダメージ音再生
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
			//球とカプセル
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

						//ダメージの情報取得
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//ダメージ処理
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//ダメージ音再生
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
		}
	}
#pragma endregion

#pragma region プレイヤーと敵の攻撃の衝突判定2
	if (!player->GetData()->isDead && boss->GetBaseAction2()) {
		BaseAction* action = boss->GetBaseAction2();
		//プレイヤーが回避またはブリンクをしていなければ衝突判定
		//攻撃の判定を行わない時間なら判定を取らない
		if ((!player->GetData()->isNoDamage) && action->GetIsCollision()) {
			const Vector3 ppos = player->GetData()->pos;
			Sphere playerSphere;
			playerSphere.center = { ppos.x, ppos.y, ppos.z, 1.0f };
			playerSphere.radius = player->GetFbxObject()->GetScale().x;

			//球とボックス
			if (action->GetUseCollision() == BaseAction::UseCollision::box) {
				std::vector<Box> bossAttackDatas;
				action->GetAttackCollisionBox(bossAttackDatas);

				int num = -1;
				for (auto& i : bossAttackDatas) {
					num++;
					if (Collision::CheckSphere2Box(playerSphere, i)) {
						Vector3 knockbackVec = ppos - (Vector3)i.point1;

						//ダメージの情報取得
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//ダメージ処理
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//ダメージ音再生
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
			//球と球
			else if (boss->GetBaseAction2()->GetUseCollision() == BaseAction::UseCollision::sphere) {
				std::vector<Sphere> bossAttackDatas;
				action->GetAttackCollisionSphere(bossAttackDatas);

				int num = -1;
				for (auto& i : bossAttackDatas) {
					num++;
					if (Collision::CheckSphere2Sphere(playerSphere, i)) {
						Vector3 knockbackVec = ppos - Vector3{ i.center.m128_f32[0], i.center.m128_f32[1], i.center.m128_f32[2] };

						//ダメージの情報取得
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//ダメージ処理
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//ダメージ音再生
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
			//球とカプセル
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

						//ダメージの情報取得
						int damageNum;
						int knockbackPower;
						int knockbackTime;
						bool isKnockbackStart;
						action->GetDamageInfo(damageNum, knockbackPower, knockbackTime, isKnockbackStart);

						//ダメージ処理
						player->Damage(damageNum, knockbackVec, knockbackPower, knockbackTime, isKnockbackStart);
						camera->ShakeStart(10, 10);
						action->SetIsCollision(false);
						action->DeleteBullet({ num });
						//ダメージ音再生
						Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

						break;
					}
				}
			}
		}
	}
#pragma endregion

#pragma region プレイヤーの攻撃と敵の衝突判定
	{
		//プレイヤーの攻撃がある場合のみ判定 
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

				//敵にヒットエフェクトを出す
				AllHitEffect::Instance()->AddParticle(collisionPos);

				//攻撃が判定を有効にしていたらダメージを与える
				if (player->GetData()->attackAction->GetIsCollisionValid()) {
					//敵にダメージ
					boss->Damage(player->GetData()->attackAction->GetAttackCollisionData().power);

					//毎フレーム多段ヒットするのを防ぐため、この攻撃の衝突判定をoffにしておく。
					player->GetData()->attackAction->AttackCollision();

					//ヒットストップ
					GameHelper::Instance()->SetSlow(0, hitStopFrame);
					//攻撃ヒット音再生
					Audio::Instance()->SoundPlayWava(Sound::SoundName::attack_hit, false, 0.1f);
				}
			}
		}
	}
#pragma endregion

#pragma region カメラのロックオンターゲット設定
	{
		//カメラがロックオンターゲットを検出している場合のみ判定
		if (camera->GetisLockonStart()) {
			//敵座標
			XMFLOAT2 pos = boss->GetCenter()->GetScreenPosition();

			//敵のスクリーン座標が検出対象範囲内なら処理
			const float targetScreenDistance = 100;
			const bool isInsideTargetScreen = (pos.x <= WindowApp::GetWindowWidth() - targetScreenDistance && pos.x >= targetScreenDistance &&
				pos.y <= WindowApp::GetWindowHeight() - targetScreenDistance && pos.y >= targetScreenDistance);

			//カメラと敵が向かい合っていたら処理
			XMMATRIX cameraFrontMatWorld;
			XMMATRIX matTrans = XMMatrixTranslation(0, 0, 1);
			cameraFrontMatWorld = XMMatrixIdentity(); // 変形をリセット
			cameraFrontMatWorld *= matTrans; // ワールド行列に平行移動を反映
			cameraFrontMatWorld *= camera->GetMatWorld();

			Vector3 cameraFrontVec = camera->GetEye() - Vector3{ cameraFrontMatWorld.r[3].m128_f32[0], cameraFrontMatWorld.r[3].m128_f32[1], cameraFrontMatWorld.r[3].m128_f32[2] };
			cameraFrontVec.normalize();
			Vector3 cameraEnemyVec = camera->GetEye() - Vector3(boss->GetCenter()->GetPosition());
			cameraEnemyVec.normalize();

			float dot = cameraFrontVec.dot(cameraEnemyVec);
			bool isCameraEnemyFront = (dot >= 0);

			if (isInsideTargetScreen && isCameraEnemyFront) {
				//ロックオン対象を確定させる
				camera->Lockon(boss->GetCenter());
				//ロックオンUI表示
				lockonUI->StartLockOnAnimation();
			}
		}
	}
#pragma endregion
}
