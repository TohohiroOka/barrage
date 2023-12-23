#include "MainEngine.h"
#include "Object/3d/InstanceObject.h"
#include "Object/2d/Sprite.h"
#include "Object/2d/DebugText.h"
#include "Particle/Emitter.h"
#include "SafeDelete.h"
#include "Pipeline/ComputeShaderManager.h"
#include "Pipeline/GraphicsPipelineManager.h"
#include "Texture/TextureManager.h"
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
	TextureManager::Finalize();
	postEffect->Finalize();
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

	//テクスチャ
	TextureManager::LoadTexture("debugfont", "Resources/" + Engine::EngineUseLoadTexture[int(Engine::EngineUseLoadTextureName::debugfont)], false);
	TextureManager::LoadTexture("white", "Resources/" + Engine::EngineUseLoadTexture[int(Engine::EngineUseLoadTextureName::white)], false);

	for (auto& i : Engine::EngineUseRTVTexture) {
		TextureManager::CreateRenderTexture(i);
	}

	int depthNum = 0;
	for (auto& i : Engine::EngineUseDSVTexture) {
		if (depthNum == 0) {
			TextureManager::CreateDepthTexture(i, { 4096 ,4096 });
		}else if(depthNum == 1) {
			TextureManager::CreateDepthTexture(i, { 0,0 });
		}
		depthNum++;
	}

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

	shadowMap = Depth::Create(Engine::EngineUseDSVTexture[int(Engine::EngineUseDSVTextureName::shadowMap)]);
	Base3D::SetLightDepthTexture(shadowMap->GetTexture());

	bloom = Bloom::Create(Engine::EngineUseRTVTexture[int(Engine::EngineUseRTVTextureName::bloom)]);
	outline = Outline::Create(Engine::EngineUseRTVTexture[int(Engine::EngineUseRTVTextureName::outline)]);
	fog = Fog::Create(Engine::EngineUseRTVTexture[int(Engine::EngineUseRTVTextureName::fog)]);
	depth = Depth::Create(Engine::EngineUseDSVTexture[int(Engine::EngineUseDSVTextureName::depth)]);

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
	bloom->Draw();
	bloom->PostDrawScene();

	outline->PreDrawScene();
	outline->Draw();
	outline->PostDrawScene();

	fog->PreDrawScene();
	fog->Draw();
	fog->PostDrawScene();

	//描画前設定
	dXCommon->PreDraw();

	ImguiDraw();
	std::vector<TextureManager*> postTex(5);
	postTex[0] = postEffect->GetTexture(PostEffect::TexType::normal);
	postTex[1] = bloom->GetTexture();
	postTex[2] = outline->GetTexture();
	postTex[3] = fog->GetTexture();
	postTex[4] = depth->GetTexture();

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
	//static float t = 0.0f;
	//static float rate = 0.0f;
	//static XMFLOAT3 color1 = {};
	//static XMFLOAT3 color2 = {};

	//ImGui::Begin("fog");
	//ImGui::SetWindowSize(ImVec2(300, 300), ImGuiCond_::ImGuiCond_FirstUseEver);

	//ImGui::SliderFloat("fog set", &t, 0.0f, 2.0f);
	//ImGui::SliderFloat("fog rate", &rate, 0.0f, 2.0f);
	//ImGui::SliderFloat("fog color1 r", &color1.x, 0.0f, 1.0f);
	//ImGui::SliderFloat("fog color1 g", &color1.y, 0.0f, 1.0f);
	//ImGui::SliderFloat("fog color1 b", &color1.z, 0.0f, 1.0f);
	//ImGui::SliderFloat("fog color2 r", &color2.x, 0.0f, 1.0f);
	//ImGui::SliderFloat("fog color2 g", &color2.y, 0.0f, 1.0f);
	//ImGui::SliderFloat("fog color2 b", &color2.z, 0.0f, 1.0f);
	//ImGui::End();

	//fog->SetStrength(t);
	//fog->SetBlendRate(rate);
	//fog->SetColor1(color1);
	//fog->SetColor2(color2);
}

void MainEngine::FrameControl()
{
	fps->FixedFps();
}
