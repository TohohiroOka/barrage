#include "OnStageTest.h"

#include <imgui.h>
#include "WindowApp.h"
#include "Input/DirectInput.h"
#include "Scene/SceneManager.h"
#include "Scene1.h"

void OnStageTestScene::Initialize()
{
	onStageDirection.Init();
	onStageDirection.StartDirection();

	//地形生成
	field = std::make_unique<Field>();

	//影用光源カメラ初期化
	lightCamera = std::make_unique<LightCamera>(Vector3{ 205, 200, 204 }, Vector3{ 205, 0, 205 });
	const float projectionSize = 1.5f;
	lightCamera->SetProjectionNum({ projectionSize * (float)WindowApp::GetWindowWidth() / 5, projectionSize * (float)WindowApp::GetWindowHeight() / 5 },
		{ -projectionSize * (float)WindowApp::GetWindowWidth() / 5, -projectionSize * (float)WindowApp::GetWindowHeight() / 5 });

	onStageDirection.SetBase3DCamera();
	Base3D::SetLightCamera(lightCamera.get());

	debugCamera = DebugCamera::Create({ 300, 40, 0 });
}

void OnStageTestScene::Update()
{
	field->Update(debugCamera->GetEye(), { 0,0,0 });

	lightCamera->Update();

	if (isNormalCamera) {
		if (DirectInput::GetInstance()->TriggerKey(DIK_RETURN)) {
			isNormalCamera = !isNormalCamera;
			Base3D::SetCamera(debugCamera.get());
		}  

		onStageDirection.Update();
		onStageDirection.SetBase3DCamera();
		if (onStageDirection.GetIsDirectEnd()) {
			if (DirectInput::GetInstance()->TriggerKey(DIK_SPACE)) {
				InterfaceScene* newScene = nullptr;
				newScene = new Scene1;
				if (newScene) { SceneManager::SetNextScene(newScene); }
			}
			else if (DirectInput::GetInstance()->TriggerKey(DIK_R)) {
				onStageDirection.StartDirection();
			}
		}
	}
	else {
		debugCamera->Update();
		Base3D::SetCamera(debugCamera.get());
		if (DirectInput::GetInstance()->TriggerKey(DIK_RETURN)) {
			isNormalCamera = !isNormalCamera;
			onStageDirection.SetBase3DCamera();
		}
	}

}

void OnStageTestScene::Draw(const int _cameraNum)
{
	field->Draw();
}

void OnStageTestScene::DrawLightView(const int _cameraNum)
{
}

void OnStageTestScene::NonPostEffectDraw(const int _cameraNum)
{
	onStageDirection.Draw();
}

void OnStageTestScene::Finalize()
{
}

void OnStageTestScene::ImguiDraw()
{

	//if (isNormalCamera) {
	//	onStageDirection.ImguiDraw();
	//}
	//else {
	//	ImGui::Begin("debug imgui");

	//	ImGui::Text("Camera Pos    [ %f : %f : %f ]", debugCamera->GetEye().x, debugCamera->GetEye().y, debugCamera->GetEye().z);
	//	ImGui::Text("Camera Target [ %f : %f : %f ]", debugCamera->GetTarget().x, debugCamera->GetTarget().x, debugCamera->GetTarget().x);

	//	ImGui::End();
	//}



}

void OnStageTestScene::FrameReset()
{
}

void OnStageTestScene::CollisionCheck()
{
}