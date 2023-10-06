#include "SceneManager.h"
#include "Scene/Scene1.h"
#include "Object/2d/PostEffect/PostEffect.h"
#include "Object/2d/PostEffect/BasePostEffect.h"
#include "Loader/JsonLoader.h"

std::unique_ptr<InterfaceScene> SceneManager::scene = nullptr;
InterfaceScene* SceneManager::nextScene = nullptr;

std::unique_ptr<SceneManager> SceneManager::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	SceneManager* instance = new SceneManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize();

	return std::unique_ptr<SceneManager>(instance);
}

void SceneManager::SetPipeline(const std::string& name,std::vector<GraphicsPipelineManager::DrawSet>& _drawSet) {
	std::vector<JsonLoader::Pipeline> addPipe{};
	JsonLoader::LoadPipeline(name, &addPipe);

	const int size = int(addPipe.size());
	_drawSet.resize(size);

	for (int i = 0; i < size; i++) {
		GraphicsPipelineManager::PEPELINE inPepeline{};
		inPepeline.name = addPipe[i].name;
		inPepeline.shaderType = addPipe[i].shaderType;
		inPepeline.inputLayoutType = addPipe[i].inputLayoutType;
		inPepeline.rtvNum = addPipe[i].rtvNum;
		inPepeline.blendMode = addPipe[i].blendMode;
		inPepeline.drawMode = addPipe[i].drawMode;
		inPepeline.drawType = addPipe[i].drawType;
		inPepeline.textureNum = addPipe[i].textureNum;
		inPepeline.rootparams = addPipe[i].rootparams;

		GraphicsPipelineManager::DrawSet addDrawSet{};
		graphicsPipeline->CreatePipeline(name, inPepeline, addDrawSet);
		_drawSet[i] = addDrawSet;
	}
}

SceneManager::~SceneManager()
{
	scene.reset();
}

void SceneManager::Initialize()
{
	CreatePipeline();

	//�J�����̏�����
	camera = Camera::Create();
	camera->SetEye({ 0,0,10 });

	//�T�E���h�p
	audio = std::make_unique<Audio>();
	//���C�g
	light = LightGroup::Create();
	light->SetDirLightActive(0, true);
	light->SetDirLightColor(0, { 1,1,1 });
	light->SetDirLightDir(0, { 0.0f, 0.0f, 1.0f, 0 });

	//�ŏ��̃V�[���ݒ�
	Scene1* firstScene = new Scene1();
	firstScene->Initialize();
	scene = std::unique_ptr<Scene1>(firstScene);
}

void SceneManager::CreatePipeline()
{
	graphicsPipeline = std::make_unique<GraphicsPipelineManager>();

	//OBJ
	{
		std::vector<GraphicsPipelineManager::DrawSet> addDrawSet{};
		SetPipeline("Object3D", addDrawSet);
		Object3d::SetPipeline(addDrawSet);
	}
	//InstanceObject
	{
		std::vector<GraphicsPipelineManager::DrawSet> addDrawSet{};
		SetPipeline("InstanceObject", addDrawSet);
		InstanceObject::SetPipeline(addDrawSet);
	}
	//HEIGHT_MAP
	{
		std::vector<GraphicsPipelineManager::DrawSet> addDrawSet{};
		SetPipeline("HeightMap", addDrawSet);
		HeightMap::SetPipeline(addDrawSet);
	}
	//PrimitiveObject3D
	{
		std::vector<GraphicsPipelineManager::DrawSet> addDrawSet{};
		SetPipeline("PrimitiveObject3D", addDrawSet);
		PrimitiveObject3D::SetPipeline(addDrawSet);
	}
	//SPRITE
	{
		std::vector<GraphicsPipelineManager::DrawSet> addDrawSet{};
		SetPipeline("Sprite", addDrawSet);
		Sprite::SetPipeline(addDrawSet);
	}
	//PARTICLE
	{
		std::vector<GraphicsPipelineManager::DrawSet> addDrawSet{};
		SetPipeline("Particle", addDrawSet);
		ParticleManager::SetPipeline(addDrawSet);
	}
	//POST_EFFECT
	{
		std::vector<GraphicsPipelineManager::DrawSet> addDrawSet{};
		SetPipeline("PostEffect", addDrawSet);
		PostEffect::SetPipeline(addDrawSet);
	}
	//BASE_POST_EFFECT
	{
		std::vector<GraphicsPipelineManager::DrawSet> addDrawSet{};
		SetPipeline("BasePostEffect", addDrawSet);
		BasePostEffect::SetPipeline(addDrawSet);
	}
}

void SceneManager::Update()
{
	//�V�[���؂�ւ�
	if (nextScene)
	{
		if (scene)
		{
			scene.reset();
			Sprite::SceneFinalize();
			//ParticleManager::SceneFinalize();
		}

		//�V�[���؂�ւ�
		scene = std::unique_ptr<InterfaceScene>(nextScene);
		nextScene = nullptr;

		//������
		scene->Initialize();
	}

	//�V�[���X�V
	scene->Update();

	//�V�[���ł̃J�����X�V
	scene->CameraUpdate(0, camera.get());

	//�J�����X�V
	camera->Update();

	Base3D::SetCamera(camera.get());
	ParticleManager::SetCamera(camera.get());

	//���C�g
	light->Update();
	scene->SetLight(light.get());

	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Base3D::SetLightGroup(light.get());
}

void SceneManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	scene->SetCmdList(cmdList);

	scene->Draw(0);
}

void SceneManager::NonPostEffectDraw()
{
	scene->NonPostEffectDraw(0);
}

void SceneManager::ImguiDraw()
{
	scene->ImguiDraw();
}

void SceneManager::GetConstbufferNum()
{
	scene->GetConstbufferNum();
}

void SceneManager::FrameReset()
{
	scene->FrameReset();
}