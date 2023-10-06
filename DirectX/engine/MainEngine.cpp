#include "MainEngine.h"
#include "Object/3d/InstanceObject.h"
#include "Object/2d/Sprite.h"
#include "Object/2d/DebugText.h"
#include "Particle/Emitter.h"
#include "SafeDelete.h"
#include "Pipeline/ComputeShaderManager.h"
#include "Pipeline/GraphicsPipelineManager.h"
#include "Texture/Texture.h"
#include "Object/3d/HeightMap.h"
#include "WindowApp.h"

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
	ComputeShaderManager::Finalize();
	DescriptorHeapManager::Finalize();
}

void MainEngine::Initialize()
{
	//directX������
	dXCommon = DirectXCommon::Create();

	//key
	input = DirectInput::GetInstance();
	input->Initialize();

	//�p�b�h
	xinput = XInputManager::GetInstance();
	xinput->Initialize();

	//Object�n�̏�����
	GraphicsPipelineManager::SetDevice(dXCommon->GetDevice());
	ObjectBase::StaticInitialize(dXCommon->GetDevice());
	LightGroup::StaticInitialize(dXCommon->GetDevice());
	ComputeShaderManager::StaticInitialize(dXCommon->GetDevice());
	DebugText::GetInstance()->Initialize();

	scene = SceneManager::Create();

	postEffect = PostEffect::Create();

	bloom = Bloom::Create();
	outline = Outline::Create();
	fog = Fog::Create();
	depth = Depth::Create();

	fps = FrameRateKeep::Create();
}

bool MainEngine::Update()
{
	input->Update();
	xinput->Update();

	//�G�X�P�[�v���~�������ꂽ�Ƃ��Q�[���I��
	if (input->PushKey(DIK_ESCAPE)) { return true; }

	//�X�V
	scene->Update();

	return false;
}

void MainEngine::Draw()
{
	//�`��
	DescriptorHeapManager::PreDraw(dXCommon->GetCmdList());
	ObjectBase::SetCmdList(dXCommon->GetCmdList());

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

	//�`��O�ݒ�
	dXCommon->PreDraw();

	ImguiDraw();
	std::vector<Texture*> postTex(5);
	postTex[0] = postEffect->GetTex(PostEffect::TexType::normal);
	postTex[1] = bloom->GetTex();
	postTex[2] = outline->GetTex();
	postTex[3] = fog->GetTex();
	postTex[4] = depth->GetTex();

	postEffect->Draw(postTex);

	//�|�X�g�G�t�F�N�g�������Ȃ��`��
	scene->NonPostEffectDraw();

	//imgui�\��
	scene->ImguiDraw();

	//�R�}���h���s
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

	ImGui::SliderFloat("slider 1", &t, 0.0f, 1.0f);
	ImGui::End();

	fog->SetStrength(t);
}

void MainEngine::FrameControl()
{
	fps->FixedFps();
}
