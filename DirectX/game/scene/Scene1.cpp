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


using namespace DirectX;

const std::array<XMFLOAT4, 2> COLOR = { XMFLOAT4{ 0.0f,0.0f,0.8f,1.0f } ,{ 0.8f,0.0f,0.0f,1.0f } };

void Scene1::Initialize()
{
	Sprite::LoadTexture("gauge", "Resources/SpriteTexture/gauge.png");

	//地形生成
	field = std::make_unique<Field>();

	player = std::make_unique<Player>();

	GameCamera::SetPlayer(player.get());
	debugCamera = DebugCamera::Create({ 300, 40, 0 });
	camera = GameCamera::Create();
	player->SetGameCamera(camera.get());
	
	Base3D::SetCamera(camera.get());

	/*Sprite::LoadTexture("amm", "Resources/amm.jpg");
	sprite = Sprite::Create("amm", {}, { 1059.0f / 5.0f,1500.0f / 5.0f });
	sprite->SetTexSize({ 1059.0f,1500.0f });
	sprite->Update();*/

	boss = std::make_unique<Boss1>();

	ParticleManager::SetCamera(camera.get());
}

void Scene1::Update()
{
	DirectInput* input = DirectInput::GetInstance();

	player->Update();
	field->Update();
	boss->Update();

	//カメラ更新
	static bool isNormalCamera = true;
	if (isNormalCamera) {
		camera->Update();
		if (DirectInput::GetInstance()->TriggerKey(DIK_RETURN)) {
			isNormalCamera = !isNormalCamera;
			Base3D::SetCamera(debugCamera.get());
		}
	}
	else {
		debugCamera->Update();
		Base3D::SetCamera(debugCamera.get());
		if (DirectInput::GetInstance()->TriggerKey(DIK_RETURN)) {
			isNormalCamera = !isNormalCamera;
			Base3D::SetCamera(camera.get());
		}
	}
}

void Scene1::Draw(const int _cameraNum)
{
	field->Draw();

	//gobject->ColliderDraw();

	player->Draw();

	boss->Draw();
}

void Scene1::NonPostEffectDraw(const int _cameraNum)
{
	//スプライト
	if (_cameraNum == 0) {
		DebugText::GetInstance()->DrawAll();
		//sprite->Draw();
	}
}

void Scene1::Finalize()
{
}

void Scene1::ImguiDraw()
{
	Vector3 ppos = player->GetPosition();

	ImGui::Begin("debug imgui");
	ImGui::SetWindowSize(ImVec2(300, 300), ImGuiCond_::ImGuiCond_FirstUseEver);

	const float cameraRota = camera->GetCameraRota().x;

	ImGui::Text("%f", cameraRota);
	ImGui::Text("%f : %f", sinf(XMConvertToRadians(cameraRota)), cosf(XMConvertToRadians(cameraRota)));
	ImGui::Text("%f : %f", sinf(XMConvertToRadians(cameraRota + 90)), cosf(XMConvertToRadians(cameraRota + 90)));
	ImGui::Text("%f : %f : %f", ppos.x, ppos.y, ppos.z);

	ImGui::Text("%d : %d ", player->GetJumpMaxNum(), player->GetJumpCount());

	ImGui::End();
}

void Scene1::FrameReset()
{
}