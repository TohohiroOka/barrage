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
	//スプライトのリソースのロード
	TextureManager::LoadTexture("titleLogo", "Resources/SpriteTexture/titleLogo.png");
	//スプライト生成
	titleLogoSprite = Sprite::Create("titleLogo", {}, { 0.5f,0.5f });
	titleLogoSprite->SetSize({ 252.f * 1.5f,59.0f * 1.5f });
	titleLogoSprite->SetPosition({ 1500.f / 2.f,100.f });
	titleLogoSprite->Update();

	pressSelectButtonUI = std::make_unique<PressSelectButtonUI>();

	//Audio::Instance()->SoundPlayWava(Sound::SoundName::msp_bgm, true, 0.1f);

	//地形生成
	field = std::make_unique<Field>();
	//プレイヤー生成
	player = std::make_unique<Player>();
	//ポータル生成
	Scene1* gameScene = new Scene1;
	const float stageSize = GameHelper::Instance()->GetStageSize();
	const float portalPosY = 11.0f;
	portals[0] = std::make_unique<Portal>(Vector3{ stageSize * 0.75f, portalPosY, stageSize / 2 }, gameScene);
	TitleScene* titleScene = new TitleScene;
	portals[1] = std::make_unique<Portal>(Vector3{ stageSize * 0.25f, portalPosY, stageSize / 2 }, titleScene);
	portals[2] = std::make_unique<Portal>(Vector3{ stageSize / 2, portalPosY, stageSize * 0.75f }, nullptr);

	//カメラ生成
	GameCamera::SetPlayer(player.get());
	debugCamera = DebugCamera::Create({ 300, 40, 0 });
	camera = std::make_unique<GameCamera>();
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

void TitleScene::Update()
{
	if (!isInputConfigMode) {
		//ポータルの目の前で入力を行えばシーン変更を開始する
		if (DirectInput::GetInstance()->TriggerKey(DIK_SPACE)) {
			for (int i = 0; i < 3; i++) {
				if (portals[i]->GetIsIntoPortal()) {
					if (portals[i]->GetChangeScene() == nullptr) {}
					else {
						SceneManager::SetNextScene(portals[i]->GetChangeScene());
					}
				}
			}
		}

		//オブジェクト更新
		player->Update();
		field->Update(player->GetData()->pos, camera->GetEye());
		for (int i = 0; i < 3; i++) {
			portals[i]->Update(player->GetFbxObject()->GetPosition(), player->GetFbxObject()->GetRotation(), player->GetData()->onGround);
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
		}
		else {
			debugCamera->Update();
			if (DirectInput::GetInstance()->TriggerKey(DIK_RETURN)) {
				isNormalCamera = !isNormalCamera;
				Base3D::SetCamera(camera.get());
			}
		}
		lightCamera->Update();

		if (DirectInput::GetInstance()->TriggerKey(DIK_TAB) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_START)) {
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

	//入力設定描画
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
#pragma region プレイヤーとポータルの衝突判定
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
				//プレイヤーを押し戻す
				player->PushBack(reject);
			}
		}
	}
#pragma endregion
}
