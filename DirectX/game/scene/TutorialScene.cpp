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
	//地形生成
	field = std::make_unique<Field>();
	//プレイヤー生成
	player = std::make_unique<Player>();
	player->GetData()->SetAllActionInput(false); //全ての行動入力を受け付けない

	//カメラ生成
	GameCamera::SetPlayer(player.get());
	debugCamera = DebugCamera::Create({ 300, 40, 0 });
	camera = std::make_unique<TitleCamera>();
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
	tutorialFunc.emplace_back([this] { return TutorialAttackUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialAvoidUpdate(); });
	tutorialFunc.emplace_back([this] { return TutorialFreeUpdate(); });
	//OKスプライト生成
	okSprite = std::make_unique<OKSprite>();
	//チュートリアルお試し行動終了後タイマー生成
	tutorialActionClearTimer = std::make_unique<Engine::Timer>();

	//行動入力設定
	actionInputConfig = std::make_unique<ActionInputConfig>();

	//遷移初期化
	SceneChangeDirection::Instance()->Init();
}

void TutorialScene::Update()
{
	if (!isInputConfigMode) {
		//暗転が完了したら次のシーンへ
		if (isSceneChangeWait && SceneChangeDirection::Instance()->IsDirectionEnd()) {
			TitleScene* titleScene = new TitleScene;
			SceneManager::SetNextScene(titleScene);
		}

		//各チュートリアルフェーズの内容更新
		tutorialFunc[int(tutorialPhase)]();

		//オブジェクト更新
		player->Update();
		field->Update(player->GetData()->pos, camera->GetEye());

		//当たり判定
		CollisionCheck();

		//カメラ更新
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
		//入力設定更新
		actionInputConfig->Update();

		if (actionInputConfig->GetIsInputConfigEnd()) { isInputConfigMode = false; }
	}

	//スプライト更新
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

	//入力設定描画
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

			//移動するほど数字を減らしていく
			if (GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash)) {
				//数字テキストの数字を減らしていく
				int runNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				runNum--;
				//指定した数を減らしきったらチュートリアルお試し行動クリア
				if (runNum <= 0) {
					runNum = 0;
					isTutorialActionClear = true;

					//OKスプライト描画開始
					okSprite->DrawStart();
					//プレイヤーの行動入力の受け付けを禁止にする
					player->GetData()->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(runNum);
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

			//移動するほど数字を減らしていく
			if (GameInputManager::TriggerInputAction(GameInputManager::Jump)) {
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
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(jumpNum);
			}
		}
	}
	//チュートリアルお試し行動をクリアしている場合
	else {
		TutorialActionClearAfterUpdate(TutorialPhase::TUTORIAL_ATTACK, SentenceData::SentenceName::TUTORIAL_ATTACK);
	}
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

			//移動するほど数字を減らしていく
			if (GameInputManager::TriggerInputAction(GameInputManager::LightAttack)) {
				//数字テキストの数字を減らしていく
				int destroyNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				destroyNum--;
				//指定した数を減らしきったらチュートリアルお試し行動クリア
				if (destroyNum <= 0) {
					destroyNum = 0;
					isTutorialActionClear = true;

					//OKスプライト描画開始
					okSprite->DrawStart();
					//プレイヤーの行動入力の受け付けを禁止にする
					player->GetData()->SetAllActionInput(false);
				}

				TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->ChangeNumber(destroyNum);
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

			//移動するほど数字を減らしていく
			if (GameInputManager::TriggerInputAction(GameInputManager::Avoid_Blink_Dash)) {
				//数字テキストの数字を減らしていく
				int avoidNum = TextManager::Instance()->GetSentece().textCreator->GetNumberText(0)->GetNumber();
				avoidNum--;
				//指定した数を減らしきったらチュートリアルお試し行動クリア
				if (avoidNum <= 0) {
					avoidNum = 0;
					isTutorialActionClear = true;

					//OKスプライト描画開始
					okSprite->DrawStart();
					//プレイヤーの行動入力の受け付けを禁止にする
					player->GetData()->SetAllActionInput(false);
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
