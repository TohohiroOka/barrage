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
