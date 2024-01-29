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

	//地形生成
	field = std::make_unique<Field>();
	//プレイヤー生成
	player = std::make_unique<Player>();
	player->GetData()->SetAllActionInput(false); //全ての行動入力を受け付けない
	player->GetData()->isNoDead = true; //死なないようにしておく

	//カメラ生成
	GameCamera::SetPlayer(player.get());
	debugCamera = DebugCamera::Create({ 300, 40, 0 });
	camera = std::make_unique<TutorialCamera>();
	camera->SetAllActionInput(false); //全ての行動入力を受け付けない
	player->SetGameCamera(camera.get());

	//影用光源カメラ初期化
	lightCamera = std::make_unique<LightCamera>(Vector3{ 205, 200, 204 }, Vector3{ 205, 0, 205 });
	const float projectionSize = 1.5f;
	lightCamera->SetProjectionNum({ projectionSize * (float)WindowApp::GetWindowWidth() / 5, projectionSize * (float)WindowApp::GetWindowHeight() / 5 },
		{ -projectionSize * (float)WindowApp::GetWindowWidth() / 5, -projectionSize * (float)WindowApp::GetWindowHeight() / 5 });

	Base3D::SetCamera(camera.get());
	Base3D::SetLightCamera(lightCamera.get());

	ParticleManager::SetCamera(camera.get());


	//チュートリアル用初期設定
	player->GetData()->SetAllActionInput(false); //動けなくしておく
	TextManager::Instance()->SentenceCreate(SentenceData::SentenceName::TUTORIAL_START);
	//各チュートリアルフェーズの内容をセット
	tutorialFunc.emplace_back([this] { return TutorialStartUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialRunUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialJumpUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialCameraUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialAttackUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialAvoidUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialFreeUpdate(); });
	//カメラチュートリアルフェーズの内容をセット
	tutorialCameraFunc.emplace_back([this] { return TutorialCameraText1(); });
	tutorialCameraFunc.emplace_back([this] { return TutorialCameraZoomEnemyBorn(); });
	tutorialCameraFunc.emplace_back([this] { return TutorialCameraEnemyBorn(); });
	tutorialCameraFunc.emplace_back([this] { return TutorialCameraReturnPos(); });
	tutorialCameraFunc.emplace_back([this] { return TutorialCameraText2Action(); });
	//OKスプライト生成
	okSprite = std::make_unique<OKSprite>();
	//チュートリアルお試し行動終了後タイマー生成
	tutorialActionClearTimer = std::make_unique<Engine::Timer>();

	//チュートリアル用ポーズ画面生成
	tutorialPause = std::make_unique<TutorialPause>();

	//遷移初期化
	SceneChangeDirection::Instance()->Init();

	lockonUI = std::make_unique<LockonUI>();
	lockonUI->Init(camera.get());
}

void TutorialScene::Update()
{
	if (!tutorialPause->GetIsPauseDraw()) {
		//各チュートリアルフェーズの内容更新
		tutorialFunc[int(tutorialPhase)]();

		//オブジェクト更新
		player->Update();
		field->Update(player->GetData()->pos, camera->GetEye());
		if (tutorialEnemy) {
			tutorialEnemy->Update();
		}

		//当たり判定
		CollisionCheck();

		//カメラ更新
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

		//ポーズ画面表示
		if (GameInputManager::TriggerInputAction(GameInputManager::Pause)) {
			tutorialPause->PauseStart();
		}

		if (!camera->GetIsLockon()) { lockonUI->EndLockOnDraw(); }

		TextManager::Instance()->UpdateSentence();
		okSprite->Update();
	}
	else {
		//ポーズ画面表示終了
		if (GameInputManager::TriggerInputAction(GameInputManager::Pause)) {
			tutorialPause->PauseEnd();
		}
		//暗転が完了したら次のシーンへ
		if (isSceneChangeWait && SceneChangeDirection::Instance()->IsDirectionEnd()) {
			SceneManager::SetNextScene(changeScene);
		}
		//選択肢の選択を終えていれば
		if (TextManager::Instance()->GetIsChoiceEnd()) {
			//選択が0番ならゲームシーン開始
			if (TextManager::Instance()->GetSelectNum(ChoicesData::ChoicesName::TUTORIAL_PAUSE_CHOICE) == 0) {
				tutorialPause->PauseEnd(); 
			}
			//選択が1番ならタイトルシーンに戻る
			else if (TextManager::Instance()->GetSelectNum(ChoicesData::ChoicesName::TUTORIAL_PAUSE_CHOICE) == 1) {
				if (!isSceneChangeWait) {
					Scene1* gameScene = new Scene1;
					changeScene = gameScene;
					isSceneChangeWait = true;
					SceneChangeDirection::Instance()->PlayFadeOut();
				}
			}
			//選択が2番なら元に戻る
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

	//スプライト更新	
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
	//敵関係の当たり判定しかないので、敵が存在しなければ抜ける
	if (!tutorialEnemy) { return; }

#pragma region プレイヤーと敵の衝突判定
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
			//プレイヤーを押し戻す
			player->PushBack(reject);
		}
	}
#pragma endregion

#pragma region プレイヤーの攻撃と敵の衝突判定
	{
		//プレイヤーの攻撃がある場合のみ判定 
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

				//敵にヒットエフェクトを出す
				AllHitEffect::Instance()->AddParticle(collisionPos);

				//攻撃が判定を有効にしていたらダメージを与える
				if (player->GetData()->attackAction->GetIsCollisionValid()) {
					//毎フレーム多段ヒットするのを防ぐため、この攻撃の衝突判定をoffにしておく。
					player->GetData()->attackAction->AttackCollision();

					//ダメージ
					tutorialEnemy->Damage();

					//ヒットストップ
					const int hitStopFrame = 5;
					GameHelper::Instance()->SetSlow(0, hitStopFrame);
					//攻撃ヒット音再生
					Audio::Instance()->SoundPlayWava(Sound::SoundName::attack_hit, false, 0.1f);
				}
			}
		}
	}
#pragma endregion

#pragma region プレイヤーと敵の攻撃の衝突判定
	//球と球
	{
		std::vector<Sphere> bossAttackDatas;
		tutorialEnemy->GetAttackCollision(bossAttackDatas);

		const Vector3 ppos = player->GetData()->pos;
		Sphere playerSphere;
		playerSphere.center = { ppos.x, ppos.y, ppos.z, 1.0f };
		playerSphere.radius = player->GetFbxObject()->GetScale().x;

		//プレイヤーダメージ判定用
		//プレイヤーが回避またはブリンクをしていなければ衝突判定
		if (!player->GetData()->isNoDamage) {
			int num = -1;
			for (auto& bossAttackData : bossAttackDatas) {
				num++;
				if (Collision::CheckSphere2Sphere(playerSphere, bossAttackData)) {
					Vector3 knockbackVec = ppos - Vector3{ bossAttackData.center.m128_f32[0], bossAttackData.center.m128_f32[1], bossAttackData.center.m128_f32[2] };

					//ダメージ処理
					player->Damage(1, knockbackVec, 5, 1, true);
					camera->ShakeStart(10, 10);
					tutorialEnemy->DeleteBullet({ num });
					//ダメージ音再生
					Audio::Instance()->SoundPlayWava(Sound::SoundName::damage, false, 0.1f);

					break;
				}
			}
		}

		//プレイヤーが回避判定をする状態なら回避判定をする
		if (player->GetData()->isEnemyAttackAvoidJudge) {
			int num = -1;
			for (auto& bossAttackData : bossAttackDatas) {
				num++;
				const float avoidCheckRadiusMulti = 75.0f;
				bossAttackData.radius *= avoidCheckRadiusMulti;
				if (Collision::CheckSphere2Sphere(playerSphere, bossAttackData)) {
					//回避処理
					player->EnemyAttackAvoid();

					break;
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
			const XMFLOAT2 pos = tutorialEnemy->GetObject3d()->GetScreenPosition();

			//敵のスクリーン座標が検出対象範囲内なら処理
			const float targetScreenDistance = 100;
			const bool isInsideTargetScreen = (pos.x <= WindowApp::GetWindowWidth() - targetScreenDistance && pos.x >= targetScreenDistance &&
				pos.y <= WindowApp::GetWindowHeight() - targetScreenDistance && pos.y >= targetScreenDistance);
			if (isInsideTargetScreen) {
				//ロックオン対象を確定させる
				camera->Lockon(tutorialEnemy->GetObject3d());
				//ロックオンUI表示
				lockonUI->StartLockOnAnimation();
			}
		}
	}
#pragma endregion
}

void TutorialScene::TutorialStartUpdate()
{
	//文章表示が終われば最初の説明へ
	if (TextManager::Instance()->GetIsSentenceEnd(SentenceData::SentenceName::TUTORIAL_START)) {
		TextManager::Instance()->SentenceCreate(SentenceData::SentenceName::TUTORIAL_RUN);
		tutorialPhase = TutorialPhase::TUTORIAL_RUN;
	}
}

void TutorialScene::TutorialRunUpdate()
{
	//チュートリアルお試し行動をクリアしていない場合
	if (!isTutorialActionClear) {
		//表示されているテキストが走る行動中に表示させるテキストなら
		if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_RUN_ACTION_TEXT].text) {
			//移動入力が不可能なら可能にしておく
			if (!player->GetData()->actionInput.isMove) {
				player->GetData()->actionInput.isMove = true;
			}
			//カメラ回転入力が不可能なら可能にしておく
			if (!camera->actionInput.isCameraMove) {
				camera->actionInput.isCameraMove = true;
			}

			//移動するほど数字を減らしていく
			if (player->GetData()->isDash) {
				//数字テキストの数字を減らしていく
				float runNum = (float)TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				runNum -= player->GetData()->moveSpeed;
				//指定した数を減らしきったらチュートリアルお試し行動クリア
				if (runNum <= 0) {
					runNum = 0;
					isTutorialActionClear = true;

					//OKスプライト描画開始
					okSprite->DrawStart();
					//プレイヤーの行動入力の受け付けを禁止にする
					player->GetData()->SetAllActionInput(false);
					//カメラの入力の受け付けを禁止にする
					camera->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber((int)runNum);
			}
		}
	}
	//チュートリアルお試し行動をクリアしている場合
	else {
		TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_JUMP, SentenceData::SentenceName::TUTORIAL_JUMP);
	}
}

void TutorialScene::TutorialJumpUpdate()
{
	//チュートリアルお試し行動をクリアしていない場合
	if (!isTutorialActionClear) {
		//表示されているテキストがジャンプ行動中に表示させるテキストなら
		if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_JUMP_ACTION_TEXT].text) {
			//移動入力が不可能なら可能にしておく
			if (!player->GetData()->actionInput.isMove) {
				player->GetData()->actionInput.isMove = true;
				player->GetData()->actionInput.isJump = true;
			}
			//カメラ回転入力が不可能なら可能にしておく
			if (!camera->actionInput.isCameraMove) {
				camera->actionInput.isCameraMove = true;
			}

			//ジャンプするほど数字を減らしていく
			if (player->GetData()->action == PlayerActionName::JUMP) {
				//数字テキストの数字を減らしていく
				int jumpNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				jumpNum--;
				//指定した数を減らしきったらチュートリアルお試し行動クリア
				if (jumpNum <= 0) {
					jumpNum = 0;
					isTutorialActionClear = true;

					//OKスプライト描画開始
					okSprite->DrawStart();
					//プレイヤーの行動入力の受け付けを禁止にする
					player->GetData()->SetAllActionInput(false);
					//カメラの入力の受け付けを禁止にする
					camera->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(jumpNum);
			}
		}
	}
	//チュートリアルお試し行動をクリアしている場合
	else {
		if (XInputManager::GetInstance()->ControllerConnectCheck()) { TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_CAMERA, SentenceData::SentenceName::TUTORIAL_CAMERA_1_PAD); }
		else { TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_CAMERA, SentenceData::SentenceName::TUTORIAL_CAMERA_1_KEY); }
	}
}

void TutorialScene::TutorialCameraUpdate()
{
	//各チュートリアルフェーズの内容更新
	tutorialCameraFunc[int(tutorialCameraPhasePhase)]();
}

void TutorialScene::TutorialAttackUpdate()
{
	//チュートリアルお試し行動をクリアしていない場合
	if (!isTutorialActionClear) {
		//表示されているテキストが攻撃行動中に表示させるテキストなら
		if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_ATTACK_ACTION_TEXT].text) {
			//移動入力が不可能なら可能にしておく
			if (!player->GetData()->actionInput.isMove) {
				player->GetData()->actionInput.isMove = true;
				player->GetData()->actionInput.isJump = true;
				player->GetData()->actionInput.isLightAttack = true;
				player->GetData()->actionInput.isStrongAttack = true;
			}
			//カメラ入力が不可能なら可能にしておく
			if (!camera->actionInput.isCameraMove) {
				camera->SetAllActionInput(true);
			}

			//攻撃でダメージを当たれる度に数字を減らしていく
			if (tutorialEnemy->GetIsDamage()) {
				//数字テキストの数字を減らしていく
				int damageNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				damageNum--;
				//指定した数を減らしきったらチュートリアルお試し行動クリア
				if (damageNum <= 0) {
					damageNum = 0;
					isTutorialActionClear = true;

					//OKスプライト描画開始
					okSprite->DrawStart();
					//プレイヤーの行動入力の受け付けを禁止にする
					player->GetData()->SetAllActionInput(false);
					//カメラの入力の受け付けを禁止にする
					camera->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(damageNum);
			}
		}
	}
	//チュートリアルお試し行動をクリアしている場合
	else {
		TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_AVOID, SentenceData::SentenceName::TUTORIAL_AVOID);
	}
}

void TutorialScene::TutorialAvoidUpdate()
{
	//チュートリアルお試し行動をクリアしていない場合
	if (!isTutorialActionClear) {
		//表示されているテキストが回避行動中に表示させるテキストなら
		if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_AVOID_ACTION_TEXT].text) {
			//移動入力が不可能なら可能にしておく
			if (!player->GetData()->actionInput.isMove) {
				player->GetData()->actionInput.isMove = true;
				player->GetData()->actionInput.isJump = true;
				player->GetData()->actionInput.isAvoid = true;
				player->GetData()->actionInput.isBlink = true;
			}
			//カメラ入力が不可能なら可能にしておく
			if (!camera->actionInput.isCameraMove) {
				camera->SetAllActionInput(true);
			}
			//敵の弾発射フラグを立てる
			if (!tutorialEnemy->GetIsBulletShot()) {
				tutorialEnemy->SetIsBulletShot(true);
			}

			//回避するほど数字を減らしていく
			if (player->GetData()->isEnemyAttackAvoid) {
				//数字テキストの数字を減らしていく
				int avoidNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				avoidNum--;
				player->GetData()->isEnemyAttackAvoid = false;
				//指定した数を減らしきったらチュートリアルお試し行動クリア
				if (avoidNum <= 0) {
					avoidNum = 0;
					isTutorialActionClear = true;

					//OKスプライト描画開始
					okSprite->DrawStart();
					//プレイヤーの行動入力の受け付けを禁止にする
					player->GetData()->SetAllActionInput(false);
					//カメラの入力の受け付けを禁止にする
					camera->SetAllActionInput(false);
					//敵の弾発射フラグを下ろす
					tutorialEnemy->SetIsBulletShot(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(avoidNum);
			}
		}
	}
	//チュートリアルお試し行動をクリアしている場合
	else {
		TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_FREE, SentenceData::SentenceName::TUTORIAL_FREE);
	}
}

void TutorialScene::TutorialFreeUpdate()
{
	//表示されているテキストが自由行動中に表示させるテキストなら
	if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_FREE_ACTION_TEXT].text) {
		//移動入力が不可能なら可能にしておく
		if (!player->GetData()->actionInput.isMove) {
			player->GetData()->SetAllActionInput(true);
		}
		//カメラ回転が不可能なら可能にしておく
		if (!camera->actionInput.isCameraMove) {
			camera->SetAllActionInput(true);
		}
		//敵の弾発射フラグを立てる
		if (!tutorialEnemy->GetIsBulletShot()) {
			tutorialEnemy->SetIsBulletShot(true);
		}
	}
}

void TutorialScene::TutorialCameraText1()
{
	//カメラ説明文章1が終了していなければ抜ける
	if (!(TextManager::Instance()->GetIsSentenceEnd(SentenceData::SentenceName::TUTORIAL_CAMERA_1_KEY) ||
		TextManager::Instance()->GetIsSentenceEnd(SentenceData::SentenceName::TUTORIAL_CAMERA_1_PAD))) {
		return;
	}

	//テキスト表示終了
	TextManager::Instance()->SentenceDrawEnd();

	//カメラを敵生成が行われる位置まで移動させる
	camera->SetZoomTargetPos(enemyBornPos);
	camera->ChangePhase(TutorialCamera::TutorialCameraPhase::ZOOM_ENEMY_BORN);

	//次のフェーズへ
	tutorialCameraPhasePhase = TutorialCameraPhasePhase::TUTORIAL_CAMERA_ZOOM_ENEMY_BORN;
}

void TutorialScene::TutorialCameraZoomEnemyBorn()
{
	//カメラのズームが終了していなければ抜ける
	if (!(camera->GetTutorialCameraPhase() == TutorialCamera::TutorialCameraPhase::ZOOM_ENEMY_BORN)) { return; }
	if (!camera->GetIsPhaseActionEnd()) { return; }

	//次のフェーズへ
	tutorialCameraPhasePhase = TutorialCameraPhasePhase::TUTORIAL_CAMERA_ENEMY_BORN;
}

void TutorialScene::TutorialCameraEnemyBorn()
{
	//敵が生成前のとき
	if (!tutorialEnemy) {
		enemyBornTimer++;
		if (enemyBornTimer <= 30) { return; }

		tutorialEnemy = std::make_unique<TutorialEnemy>(enemyBornPos, player->GetData());

		enemyBornTimer = 0;
	}
	else {
		enemyBornTimer++;
		if (enemyBornTimer <= 30) { return; }

		//カメラを元の位置まで戻す
		camera->ChangePhase(TutorialCamera::TutorialCameraPhase::ZOOM_END_RETURN);

		//次のフェーズへ
		tutorialCameraPhasePhase = TutorialCameraPhasePhase::TUTORIAL_CAMERA_RETURN_POS;
	}
}

void TutorialScene::TutorialCameraReturnPos()
{
	//カメラの元に戻る処理が終了していなければ抜ける
	if (!(camera->GetTutorialCameraPhase() == TutorialCamera::TutorialCameraPhase::ZOOM_END_RETURN)) { return; }
	if (!camera->GetIsPhaseActionEnd()) { return; }

	//カメラ説明文章2を生成する
	TextManager::Instance()->SentenceCreate(SentenceData::SentenceName::TUTORIAL_CAMERA_2);

	//カメラ状態を通常状態に戻す
	camera->ChangePhase(TutorialCamera::TutorialCameraPhase::NORMAL);

	//次のフェーズへ
	tutorialCameraPhasePhase = TutorialCameraPhasePhase::TUTORIAL_CAMERA_TEXT_2_ACTION;
}

void TutorialScene::TutorialCameraText2Action()
{
	//チュートリアルお試し行動をクリアしていない場合
	if (!isTutorialActionClear) {
		//表示されているテキストがロックオン行動中に表示させるテキストなら
		if (TextManager::Instance()->GetSentece().text == TextData::textData[(int)TextData::TextName::TUTORIAL_CAMERA_ACTION_TEXT].text) {
			//移動入力が不可能なら可能にしておく
			if (!player->GetData()->actionInput.isMove) {
				player->GetData()->actionInput.isMove = true;
				player->GetData()->actionInput.isJump = true;
			}
			//カメラ入力が不可能なら可能にしておく
			if (!camera->actionInput.isCameraMove) {
				camera->SetAllActionInput(true);
			}

			//ロックオンするほど数字を減らしていく
			if (camera->GetIsLockon()) {
				//数字テキストの数字を減らしていく
				int lockonNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				lockonNum--;
				//指定した数を減らしきったらチュートリアルお試し行動クリア
				if (lockonNum <= 0) {
					lockonNum = 0;
					isTutorialActionClear = true;

					//OKスプライト描画開始
					okSprite->DrawStart();
					//プレイヤーの行動入力の受け付けを禁止にする
					player->GetData()->SetAllActionInput(false);
					//カメラの入力の受け付けを禁止にする
					camera->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(lockonNum);
			}
		}
	}
	//チュートリアルお試し行動をクリアしている場合
	else {
		TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_ATTACK, SentenceData::SentenceName::TUTORIAL_ATTACK);
	}
}

void TutorialScene::TutorialActionClearAfterUpdate(TutorialPhase nextTutorialPhase, SentenceData::SentenceName nextTutorialSentenceName)
{
	//OKスプライトの表示が終了したらクリア後タイマー更新
	if (okSprite->GetIsDraw() && !okSprite->GetIsScaleChange()) {
		tutorialActionClearTimer->Update();
	}
	//チュートリアルお試し行動終了後タイマーが指定した時間になったら次のチュートリアルフェーズへ
	const int tutorialActionClearAfterTime = 60;
	if (*tutorialActionClearTimer.get() >= tutorialActionClearAfterTime) {
		isTutorialActionClear = false;
		tutorialActionClearTimer->Reset();
		okSprite->SetIsDraw(false);
		tutorialPhase = nextTutorialPhase;
		TextManager::Instance()->SentenceCreate(nextTutorialSentenceName);
	}
}
