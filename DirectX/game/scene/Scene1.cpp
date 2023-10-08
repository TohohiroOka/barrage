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
	camera = GameCamera::Create();

	const std::string jimen = "jimen.png";
	const std::string kabe = "kabe.png";
	gmodel = TerrainModel::Create("heightmap3.bmp", 25.0f,
		{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 1.0f, jimen, kabe);

	gobject = HeightMap::Create(gmodel.get());
	gobject->SetScale({ 15.0f ,15.0f ,15.0f });
	gobject->UpdateWorldMatrix();

	gobject->DeleteCollider();

	Base3D::SetCamera(camera.get());

	// �R���C�_�[�̒ǉ�
	MeshCollider* collider = new MeshCollider;
	gobject->SetCollider(collider);
	collider->ConstructTriangles(gobject->GetModel());
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);

	Sprite::LoadTexture("amm", "Resources/amm.jpg");
	sprite = Sprite::Create("amm");
	sprite->SetTexSize({ 1059.0f,1500.0f });
	sprite->SetSize({ 1059.0f / 5.0f,1500.0f / 5.0f });
	sprite->Update();

	player = std::make_unique<Player>();
	GameCamera::SetPlayer(player.get());

	fbxmodel = FbxModel::Create("boneTest");
	fbx = Fbx::Create(fbxmodel.get());
	fbx->SetPosition({ 500.f,100.0f,500.0f });
	fbx->SetScale({ 15.0f ,15.0f ,15.0f });
	fbx->SetAnimation(true);

	ParticleManager::SetCamera(camera.get());
}

void Scene1::Update()
{
	DirectInput* input = DirectInput::GetInstance();

	player->Update();
	gobject->Update();
	fbx->Update();

	//�J�����X�V
	camera->Update();
}

void Scene1::Draw(const int _cameraNum)
{
	gobject->Draw();

	//gobject->ColliderDraw();

	player->Draw();

	fbx->Draw();
}

void Scene1::NonPostEffectDraw(const int _cameraNum)
{
	//�X�v���C�g
	if (_cameraNum == 0) {
		DebugText::GetInstance()->DrawAll();
		sprite->Draw();
	}
}

void Scene1::Finalize()
{
}

void Scene1::ImguiDraw()
{
	Vector3 ppos = player->GetPos();

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