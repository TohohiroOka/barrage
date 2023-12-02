#include "Boss1OnStage.h"
#include "Object/3d/Base3D.h"
#include "WindowApp.h"

void Boss1OnStage::Init()
{
	Sprite::LoadTexture("first_name", "Resources/SpriteTexture/firstname.png");
	Sprite::LoadTexture("second_name", "Resources/SpriteTexture/secondname.png");
	Sprite::LoadTexture("letterbox", "Resources/SubTexture/white1x1.png");

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
	camera->SetPan({ 100,15,60 }, { 500,15,200 }, { 0,-100,+300 }, 180, CutSceneCamera::EASE_LERP);
	//プレイヤーキャラクターをズーム表示
	camera->SetZoom({ 250,15,100 }, { 250,0,50 }, { 250,10,30 }, 180, CutSceneCamera::EASE_LERP);
	//敵をズーム表示
	camera->SetZoom({ 250,50,400 }, { 250,50,450 }, { 250,60,450 }, 60, CutSceneCamera::EASE_LERP);
	//敵全体表示&ボスの名前表示
	camera->SetTrack({ 250,60,450 }, { 250,50,400 }, { 250,10,250 }, 15, CutSceneCamera::EASE_IN_QUAD);
	camera->SetTrack({ 250,60,450 }, { 250,10,250 }, { 250,5,200 }, 120, CutSceneCamera::EASE_LERP);
}

void Boss1OnStage::Update()
{
	if (camera->GetIsCameraMoveEnd()) { return; }
	camera->Update();

	if (camera->GetUsingCameraNum() == 3) { isDisplayBossText = true; }

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

	letterBox->Update();
	letterBoxUnder->Update();
}

void Boss1OnStage::Draw()
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
