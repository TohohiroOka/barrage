#include "MainEngine.h"
#include "Object/3d/InstanceObject.h"
#include "Object/2d/Sprite.h"
#include "Object/2d/DebugText.h"
#include "Particle/Emitter.h"
#include "SafeDelete.h"
#include "Pipeline/ComputeShaderManager.h"
#include "Pipeline/GraphicsPipelineManager.h"
#include "Texture/Texture.h"
#include "Object/3d/FbxModel.h"
#include "system/GameInputManager.h"
#include "system/ActionInputConfig.h"
#include "WindowApp.h"
#include "GameHelper.h"

using namespace DirectX;
using namespace Microsoft::WRL;

MainEngine::~MainEngine()
{
	DebugText::Finalize();
	scene.reset();
	//DrawLine::Finalize();
	Sprite::Finalize();
	//Fbx::Finalize();
	ParticleManager::Finalize();
	postEffect->Finalize();
	shadowMap->Finalize();
	ComputeShaderManager::Finalize();
	DescriptorHeapManager::Finalize();
}

void MainEngine::Initialize()
{
	//directX初期化
	dXCommon = DirectXCommon::Create();

	//key
	input = DirectInput::GetInstance();
	input->Initialize();

	//パッド
	xinput = XInputManager::GetInstance();
	xinput->Initialize();

	//Object系の初期化
	GraphicsPipelineManager::SetDevice(dXCommon->GetDevice());
	ObjectBase::StaticInitialize(dXCommon->GetDevice());
	LightGroup::StaticInitialize(dXCommon->GetDevice());
	ComputeShaderManager::StaticInitialize(dXCommon->GetDevice());
	DebugText::GetInstance()->Initialize();
	FbxModel::StaticInitialize(dXCommon->GetDevice());

	//ゲーム入力
	GameInputManager::Initialize();
	ActionInputConfig::LoadTexture();

	scene = SceneManager::Create();

	postEffect = PostEffect::Create();

	shadowMap = Depth::Create({ 4096 ,4096 });
	Base3D::SetLightDepthTexture(shadowMap->GetTex());

	bloom = Bloom::Create();
	outline = Outline::Create();
	fog = Fog::Create();
	depth = Depth::Create();

	fps = FrameRateKeep::Create();
}

bool MainEngine::Update()
{
	GameHelper::Instance()->Update();

	input->Update();
	xinput->Update();

	//エスケープか×が押されたときゲーム終了
	if (input->PushKey(DIK_ESCAPE)) { return true; }

	//更新
	scene->Update();

	return false;
}

void MainEngine::Draw()
{
	//描画
	DescriptorHeapManager::PreDraw(dXCommon->GetCmdList());
	ObjectBase::SetCmdList(dXCommon->GetCmdList());

	shadowMap->PreDrawScene();
	scene->DrawLightView(dXCommon->GetCmdList());
	shadowMap->PostDrawScene();

	postEffect->PreDrawScene();
	scene->Draw(dXCommon->GetCmdList());
	postEffect->PostDrawScene();

	depth->PreDrawScene();
	scene->Draw(dXCommon->GetCmdList());
	depth->PostDrawScene();

	bloom->PreDrawScene();
	bloom->Draw(postEffect->GetTex(PostEffect::TexType::bloom));
	bloom->PostDrawScene();

	outline->PreDrawScene();
	outline->Draw(postEffect->GetTex(PostEffect::TexType::outline));
	outline->PostDrawScene();

	fog->PreDrawScene();
	fog->Draw(depth->GetTex());
	fog->PostDrawScene();

	//描画前設定
	dXCommon->PreDraw();

	ImguiDraw();
	std::vector<Texture*> postTex(5);
	postTex[0] = postEffect->GetTex(PostEffect::TexType::normal);
	postTex[1] = bloom->GetTex();
	postTex[2] = outline->GetTex();
	postTex[3] = fog->GetTex();
	postTex[4] = depth->GetTex();

	postEffect->Draw(postTex);

	//ポストエフェクトをかけない描画
	scene->NonPostEffectDraw();

	//imgui表示
	scene->ImguiDraw();

	//コマンド実行
	dXCommon->PostDraw();

	scene->FrameReset();

	ObjectBase::ReleaseCmdList();
	GraphicsPipelineManager::ResetDrawSet();
}

void MainEngine::ImguiDraw()
{
	static float t = 0;
	ImGui::Begin("debug imgui");
	ImGui::SetWindowSize(ImVec2(300, 300), ImGuiCond_::ImGuiCond_FirstUseEver);

	ImGui::SliderFloat("slider 1", &t, 0.0f, 2.0f);
	ImGui::End();

	fog->SetStrength(t);
}

void MainEngine::FrameControl()
{
	fps->FixedFps();
}
