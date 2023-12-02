#include "TitleScene.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "engine/Scene/SceneManager.h"
#include "scene/Scene1.h"
#include "cutscene/SceneChangeDirection.h"

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	//スプライトのリソースのロード
	Sprite::LoadTexture("titleLogo", "Resources/SpriteTexture/titleLogo.png");
	Sprite::LoadTexture("pab", "Resources/SpriteTexture/pressanybutton.png");
	Sprite::LoadTexture("gamestart", "Resources/SpriteTexture/gamestart.png");
	Sprite::LoadTexture("config", "Resources/SpriteTexture/config.png");
	Sprite::LoadTexture("exitgame", "Resources/SpriteTexture/exitgame.png");
	//スプライト生成
	titleLogoSprite = Sprite::Create("titleLogo", {}, { 0.5f,0.5f });
	titleLogoSprite->SetSize({ 252.f * 1.5f,59.0f * 1.5f });
	titleLogoSprite->SetPosition({ 1500.f / 2.f,200.f });
	titleLogoSprite->Update();
	pressAnyButtonSprite = Sprite::Create("pab", {}, { 512.f,32.f });
	pressAnyButtonSprite->SetTexSize({ 512.f,32.0f });
	pressAnyButtonSprite->SetAnchorpoint({ 0.5f,0.5f });
	pressAnyButtonSprite->SetPosition({ 1500.f / 2.f,550.f });
	pressAnyButtonSprite->Update();
	gamestartSprite = Sprite::Create("gamestart", {}, { 320.f,32.f });
	gamestartSprite->SetTexSize({ 320.f,32.f });
	gamestartSprite->SetAnchorpoint({ 0.5f,0.5f });
	gamestartSprite->SetPosition({ 1500.f / 2,OPTIONS_START_Y });
	gamestartSprite->Update();
	configSprite = Sprite::Create("config", {}, { 320.f,32.f });
	configSprite->SetTexSize({ 320.f,32.f });
	configSprite->SetAnchorpoint({ 0.5f,0.5f });
	configSprite->SetPosition({ 1500.f / 2, OPTIONS_START_Y + OPTIONS_DISTANCE_Y });
	configSprite->Update();
	exitgameSprite = Sprite::Create("exitgame", {}, { 320.f,32.f });
	exitgameSprite->SetTexSize({ 320.f,32.f });
	exitgameSprite->SetAnchorpoint({ 0.5f,0.5f });
	exitgameSprite->SetPosition({ 1500.f / 2,OPTIONS_START_Y + (OPTIONS_DISTANCE_Y * 2) });
	exitgameSprite->Update();

	//カメラ初期化
	debugCamera = DebugCamera::Create({ 300, 40, 0 });
	lightCamera.reset(new LightCamera({ -50, 20, -50 }));
	lightCamera->SetProjectionNum({ 360, 300 }, { -360, -100 });
	//カメラ設定
	Base3D::SetCamera(debugCamera.get());
	Base3D::SetLightCamera(lightCamera.get());

	//強調表示初期化
	choiceDrawer=std::make_unique<ChoiceEmphasisDrawer>();
	choiceDrawer->Initialize();

	//遷移初期化
	SceneChangeDirection::Init();

	actionInputConfig = std::make_unique<ActionInputConfig>();
}

void TitleScene::Update()
{
	//カメラ更新
	debugCamera->Update();
	lightCamera->Update();

	if (isPressed) {
		if (isConfigMode) {
			//入力設定更新
			actionInputConfig->Update();
			if (actionInputConfig->GetIsInputConfigEnd()) { 
				isConfigMode = false;
				isSelected = false;
			}
		}
		else {
			if (!isSelected) {
				if (IsUp() && selecting != PLAYER_SELECT::SELECT_STARTGAME) {
					selecting = PLAYER_SELECT(int(selecting) - 1);
				}
				else if (IsDown() && selecting != PLAYER_SELECT::SELECT_CONFIG) {
					selecting = PLAYER_SELECT(int(selecting) + 1);
				}
			}

			choiceDrawer->SetEmphasisPos(1500.f / 2.f, OPTIONS_START_Y + (OPTIONS_DISTANCE_Y * float(selecting)), 550.f, 80.f);

			//決定キーが押されたら
			if (IsEnter()) {
				choiceDrawer->PlayChoiseAnimation();
				//カーソル移動をロック
				isSelected = true;
			}

			if (choiceDrawer->IsChooseAnimEnd()) {
				switch (selecting)
				{
				case TitleScene::PLAYER_SELECT::SELECT_STARTGAME:
					isSceneChangeWait = true;
					SceneChangeDirection::PlayFadeOut();
					break;
				case TitleScene::PLAYER_SELECT::SELECT_CONFIG:
					isConfigMode = true;
					actionInputConfig->Reset();
					break;
				case TitleScene::PLAYER_SELECT::SELECT_EXIT:

					break;
				default:
					break;
				}
			}

			if (isSceneChangeWait && SceneChangeDirection::IsDirectionEnd()) {
				Scene1* gameScene = nullptr;
				gameScene = new Scene1;
				SceneManager::SetNextScene(gameScene);
			}
		}



	}
	else {
		//なにかしらのボタンが押されたら選択肢表示
		if (DirectInput::GetInstance()->ReleaseKey(DIK_SPACE) ||
			XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A)) {
			isPressed = true;
		}
	}



	choiceDrawer->Update();

	SceneChangeDirection::Update();
}

void TitleScene::Draw(const int _cameraNum)
{

}

void TitleScene::DrawLightView(const int _cameraNum)
{
}

void TitleScene::NonPostEffectDraw(const int _cameraNum)
{
	titleLogoSprite->Draw();

	if (!isConfigMode) {
		if (isPressed) {
			choiceDrawer->Draw();
			gamestartSprite->Draw();
			configSprite->Draw();
			//exitgameSprite->Draw();
		}
		else {
			pressAnyButtonSprite->Draw();
		}
	}
	else {
		actionInputConfig->Draw();
	}

	SceneChangeDirection::Draw();
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

bool TitleScene::IsUp()
{
	return DirectInput::GetInstance()->TriggerKey(DIK_UP) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_UP) ||
		XInputManager::GetInstance()->TriggerLeftStickY(true);
}

bool TitleScene::IsDown()
{
	return DirectInput::GetInstance()->TriggerKey(DIK_DOWN) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_DOWN) ||
		XInputManager::GetInstance()->TriggerLeftStickY(false);
}

bool TitleScene::IsEnter()
{
	bool isEnter = (DirectInput::GetInstance()->TriggerKey(DIK_SPACE) ||
		XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_A));

	return isEnter;
}
