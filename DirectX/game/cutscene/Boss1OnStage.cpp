#include "Boss1OnStage.h"
#include "Object/3d/Base3D.h"
#include "WindowApp.h"

void Boss1OnStage::Init()
{
	//モデル読み込み
	playerModel = FbxModel::Create("player");
	playerObject = Fbx::Create(playerModel.get());
	boss1Model = std::make_unique<Boss1Model>();
	boss1Model->SetPosition(bossObjectPos);
	boss1Model->Update();
	boss1Model->SetAnimation(int(Boss1Model::Movement::back_end));
	boss1Model->AnimationReset();
	boss1Model->SetAnimation(int(Boss1Model::Movement::back_start));
	boss1Model->AnimationReset();
	boss1Model->SetIsRoop(false);

	playerObject->SetPosition(playerObjectPos);
	playerObject->SetUseAnimation(5);
	playerObject->SetShadowMap(true);
	playerObject->SetAnimation(true);
	playerObject->SetIsBoneDraw(true);
	playerObject->SetScale({ 5,5,5 });

	TextureManager::LoadTexture("first_name", "Resources/SpriteTexture/firstname.png");
	TextureManager::LoadTexture("second_name", "Resources/SpriteTexture/secondname.png");
	TextureManager::LoadTexture("letterbox", "Resources/SubTexture/white1x1.png");

	firstName = Sprite::Create("first_name", {});
	firstName->SetAnchorpoint({ 0.5,0.5 });
	firstName->SetColor({ 1,1,1,0 });
	firstName->SetPosition({ (float)WindowApp::GetWindowWidth() / 2,(float)WindowApp::GetWindowHeight() / 4 });
	firstName->SetSize({ 640.f,64.f });
	secondName = Sprite::Create("second_name", {});
	secondName->SetAnchorpoint({ 0.5,0.5 });
	secondName->SetColor({ 1,1,1,0 });
	secondName->SetSize({ 320.f,32.f });
	secondName->SetPosition({ (float)WindowApp::GetWindowWidth() / 2,(float)WindowApp::GetWindowHeight() / 4 + 64 });

	letterBox = Sprite::Create("letterbox", {});
	letterBox->SetPosition({ 0,0 });
	letterBox->SetSize({ (float)WindowApp::GetWindowWidth() ,80 });
	letterBox->SetColor({ 0,0,0,1 });
	letterBoxUnder = Sprite::Create("letterbox", {});
	letterBoxUnder->SetPosition({ 0,(float)WindowApp::GetWindowHeight() - 80 });
	letterBoxUnder->SetSize({ (float)WindowApp::GetWindowWidth() ,80 });
	letterBoxUnder->SetColor({ 0,0,0,1 });

	//演出用カメラワーク初期化
	camera = std::make_unique<CutSceneCamera>();
	camera->Init();
	//フィールド全域撮影
	//camera->SetPan({ 100,20,-100 }, { 100,20,-100 }, { 0,-10,+30 }, 240, CutSceneCamera::EASE_LERP);
	camera->SetPan({ 0,15,0 }, { 200,15,0 }, { 0,-10,+30 }, 120, CutSceneCamera::EASE_LERP);
	//プレイヤーキャラクターをズーム表示
	camera->SetZoom({ 100,15,50 }, { 100,0,30 }, { 100,10,20 }, 60, CutSceneCamera::EASE_LERP);
	//敵をズーム表示
	camera->SetZoom({ 100,15,150 }, { 100,25,160 }, { 100,20,150 }, 30, CutSceneCamera::EASE_LERP);
	//敵全体表示&ボスの名前表示
	camera->SetTrack(bossObjectPos, { 100,15,150 }, { 100,10,130 }, 15, CutSceneCamera::EASE_IN_QUAD);
	camera->SetTrack(bossObjectPos, { 100,10,130 }, { 100,5,120 }, 60, CutSceneCamera::EASE_LERP);

	Audio::Instance()->SoundPlayWava(Sound::SoundName::onstage, false, 0.1f);
}

void Boss1OnStage::Update()
{
	if (camera->GetIsCameraMoveEnd()) { return; }
	camera->Update();

	if (camera->GetUsingCameraNum() == 2) {
		boss1Model->SetMovement(int(Boss1Model::Movement::back_start));
	}
	if (camera->GetUsingCameraNum() == 3) { 
		isDisplayBossText = true; 
	}

	if (isDisplayBossText) {
		if (frame < TEXTANIM_MAXFRAME) { frame++; }
		float rate = float(frame) / float(TEXTANIM_MAXFRAME);
		firstName->SetColor({ 1,1,1,1.f * rate });
		secondName->SetColor({ 1,1,1,1.f * rate });
		firstName->SetSize({ 640.f * (1.5f - (0.5f * rate)),64.f * (1.5f - (0.5f * rate)) });
		secondName->SetSize({ 320.f * (1.5f - (0.5f * rate)),32.f * (1.5f - (0.5f * rate)) });
		firstName->Update();
		secondName->Update();
	}

	playerObject->Update();
	boss1Model->Update();

	letterBox->Update();
	letterBoxUnder->Update();
}

void Boss1OnStage::Draw3D()
{
	playerObject->Draw();
	boss1Model->Draw();
}

void Boss1OnStage::Draw2D()
{
	letterBox->Draw();
	letterBoxUnder->Draw();

	if (!isDisplayBossText) { return; }
	firstName->Draw();
	secondName->Draw();
}

void Boss1OnStage::SetBase3DCamera()
{
	Base3D::SetCamera(camera->GetCamera());
}

void Boss1OnStage::StartDirection()
{
	camera->StartCameraMove();
	frame = 0;
	isDisplayBossText = false;
}

void Boss1OnStage::Reset()
{
	camera->StartCameraMove();
	frame = 0;
	isDisplayBossText = false;
}

void Boss1OnStage::FrameReset()
{
	playerObject->FrameReset();
	boss1Model->RrameReset();
}

void Boss1OnStage::DrawLightView()
{
	playerObject->DrawLightView();
	boss1Model->DrawLightView();
}
