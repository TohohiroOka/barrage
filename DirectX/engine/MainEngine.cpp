#include "MainEngine.h"
#include "Object/3d/InstanceObject.h"
#include "Object/2d/Sprite.h"
#include "Object/2d/DebugText.h"
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
	for (auto& i : render) {
		i.reset();
	}
	mainSprite.reset();
	postEffect.reset();
	bloom.reset();
	outline.reset();
	fog.reset();
	depthRender.reset();
	shadowMapRender.reset();
	ComputeShaderManager::Finalize();
	TextureManager::Finalize();
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
	for (auto& i : Engine::EngineUseRTVCreateTexture) {
		TextureManager::CreateRenderTexture(i);
	}

	int depthNum = 0;
	for (auto& i : Engine::EngineUseDSVTexture) {
		if (depthNum == 0) {
			TextureManager::CreateDepthTexture(i, { 4096 ,4096 });
		}
		else if (depthNum == 1) {
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
	BaseRender::StaticInitialize(dXCommon->GetDevice(), dXCommon->GetCmdList());
	DepthRender::StaticInitialize(dXCommon->GetDevice(), dXCommon->GetCmdList());

	//ゲーム入力
	GameInputManager::Initialize();
	ActionInputConfig::LoadTexture();

	scene = SceneManager::Create();

	postEffect = PostEffect::Create();

	using namespace Engine;
	for (int i = 0; i<int(EngineUseRTVCreateTextureName::size); i++) {
		render[i] = std::make_unique<BaseRender>(EngineUseRTVCreateTexture[i]);
	}

	depthRender = std::make_unique<DepthRender>(EngineUseDSVTexture[int(EngineUseDSVTextureName::depth)]);
	shadowMapRender = std::make_unique<DepthRender>(EngineUseDSVTexture[int(EngineUseDSVTextureName::shadowMap)]);
	Base3D::SetLightDepthTexture(shadowMapRender->GetTexture());

	bloom = Bloom::Create(EngineUseRTVTexture[int(EngineUseRTVTextureName::bloom_before)]);
	outline = Outline::Create(EngineUseRTVTexture[int(EngineUseRTVTextureName::outline_before)]);
	fog = Fog::Create(EngineUseRTVTexture[int(EngineUseDSVTextureName::depth)]);

	fps = FrameRateKeep::Create();

	mainSprite = Sprite::Create("main_after", {});
	mainSprite->Update();
}

bool MainEngine::Update()
{
	GameHelper::Instance()->Update();

	input->Update();
	xinput->Update();

	//更新
	scene->Update();

	//各シーンから終了リクエストがあればゲームループ終了
	if (scene->GetIsEndRequest()) { return true; }

	return false;
}

void MainEngine::Draw()
{
	//描画
	DescriptorHeapManager::PreDraw(dXCommon->GetCmdList());
	ObjectBase::SetCmdList(dXCommon->GetCmdList());

	shadowMapRender->PreDrawScene();
	scene->DrawLightView(dXCommon->GetCmdList());
	shadowMapRender->PostDrawScene();

	postEffect->PreDrawScene();
	scene->Draw(dXCommon->GetCmdList());
	postEffect->PostDrawScene();

	depthRender->PreDrawScene();
	scene->Draw(dXCommon->GetCmdList());
	depthRender->PostDrawScene();

	using namespace Engine;
	render[int(EngineUseRTVCreateTextureName::bloom_after)]->PreDrawScene();
	bloom->Draw();
	render[int(EngineUseRTVCreateTextureName::bloom_after)]->PostDrawScene();

	render[int(EngineUseRTVCreateTextureName::outline_after)]->PreDrawScene();
	outline->Draw();
	render[int(EngineUseRTVCreateTextureName::outline_after)]->PostDrawScene();

	render[int(EngineUseRTVCreateTextureName::fog_after)]->PreDrawScene();
	fog->Draw();
	render[int(EngineUseRTVCreateTextureName::fog_after)]->PostDrawScene();

	render[int(EngineUseRTVCreateTextureName::main_after)]->PreDrawScene();
	std::vector<TextureManager*> postTex(5);
	postTex[0] = postEffect->GetTexture(PostEffect::TexType::normal);
	postTex[1] = render[int(EngineUseRTVCreateTextureName::bloom_after)]->GetTexture();
	postTex[2] = render[int(EngineUseRTVCreateTextureName::outline_after)]->GetTexture();
	postTex[3] = render[int(EngineUseRTVCreateTextureName::fog_after)]->GetTexture();
	postTex[4] = depthRender->GetTexture();

	postEffect->Draw(postTex);

	render[int(EngineUseRTVCreateTextureName::main_after)]->PostDrawScene();

	//描画前設定
	dXCommon->PreDraw();

	mainSprite->Draw();

	//ポストエフェクトをかけない描画
	scene->NonPostEffectDraw();

	ImguiDraw();

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
