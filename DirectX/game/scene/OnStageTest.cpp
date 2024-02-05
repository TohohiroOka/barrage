#include "OnStageTest.h"

#include <imgui.h>
#include "WindowApp.h"
#include "Input/DirectInput.h"
#include "Scene/SceneManager.h"
#include "Scene1.h"
#include "cutscene/SceneChangeDirection.h"

void OnStageTestScene::Initialize()
{
	onStageDirection = std::make_unique<Boss1OnStage>();
	onStageDirection->Init();
	onStageDirection->StartDirection();

	//地形生成
	field = std::make_unique<Field>();

	//影用光源カメラ初期化
	lightCamera = std::make_unique<LightCamera>(Vector3{ 205, 200, 204 }, Vector3{ 205, 0, 205 });
	const float projectionSize = 1.5f;
	lightCamera->SetProjectionNum({ projectionSize * (float)WindowApp::GetWindowWidth() / 5, projectionSize * (float)WindowApp::GetWindowHeight() / 5 },
		{ -projectionSize * (float)WindowApp::GetWindowWidth() / 5, -projectionSize * (float)WindowApp::GetWindowHeight() / 5 });

	onStageDirection->SetBase3DCamera();
	Base3D::SetLightCamera(lightCamera.get());

	SceneChangeDirection::Instance()->PlayFadeIn();

	
}

void OnStageTestScene::Update()
{
	lightCamera->Update();

	onStageDirection->Update();
	onStageDirection->SetBase3DCamera();
	if (onStageDirection->GetIsDirectEnd() && !isChangeSceneWait) {
		SceneChangeDirection::Instance()->PlayFadeOut();
		isChangeSceneWait = true;
	}
	if (isChangeSceneWait && SceneChangeDirection::Instance()->IsDirectionEnd()) {
		InterfaceScene* newScene = nullptr;
		newScene = new Scene1;
		if (newScene) { SceneManager::SetNextScene(newScene); }
	}

	SceneChangeDirection::Instance()->Update();
}

void OnStageTestScene::Draw(const int _cameraNum)
{
	field->Draw();
	onStageDirection->Draw3D();
}

void OnStageTestScene::DrawLightView(const int _cameraNum)
{
	onStageDirection->DrawLightView();
}

void OnStageTestScene::NonPostEffectDraw(const int _cameraNum)
{
	onStageDirection->Draw2D();

	SceneChangeDirection::Instance()->Draw();
}

void OnStageTestScene::ImguiDraw()
{

	if (isNormalCamera) {
		//onStageDirection->ImguiDraw();
	}
	else {

	}



}

void OnStageTestScene::FrameReset()
{
	field->FrameReset();
	onStageDirection->FrameReset();
}

void OnStageTestScene::CollisionCheck()
{
}